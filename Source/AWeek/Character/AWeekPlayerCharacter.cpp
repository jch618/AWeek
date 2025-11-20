// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Pakour/AWeekPakourComponent.h"
#include "Stamina/AWeekStaminaComponent.h"
#include "../Player/Hunger/AWeekHungerComponent.h"
#include "../Player/Weapon/AWeekWeaponComponent.h"
#include "Perception/AISense_Hearing.h"

#include "../System/DamageSystemComponent.h"
#include "../Input/AWeekGameInput.h"
#include "../System/DaySystem/AWeekDaySystem.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/Interfaces/AWeekInteractionInterface.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Components/AWeekLootComponent.h"
#include "AWeek/Components/AWeekPlayerInventoryComponent.h"
#include "AWeek/World/AWeekPickupItem.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeek/UI/Controller/AWeekInventoryController.h"
#include "AWeek/Settings/AWeekGameUserSettings.h"

DEFINE_LOG_CATEGORY(AWeekPlayerCharacter);

AAWeekPlayerCharacter::AAWeekPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->bHiddenInGame=false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = mJumpVelocity;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	mPakour = CreateDefaultSubobject<UAWeekPakourComponent>(TEXT("Pakour"));

	PlayerInventoryComponent = CreateDefaultSubobject<UAWeekPlayerInventoryComponent>(TEXT("PlayerInventory"));
	CraftingComponent = CreateDefaultSubobject<UAWeekCraftingComponent>(TEXT("CraftingComponent"));
	
	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 250.0f;
	mStamina = CreateDefaultSubobject<UAWeekStaminaComponent>(TEXT("Stamina"));
	mWeapon = CreateDefaultSubobject<UAWeekWeaponComponent>(TEXT("Weapon"));
	mHunger = CreateDefaultSubobject<UAWeekHungerComponent>(TEXT("Hunger"));
	mDamageSystem = CreateDefaultSubobject<UDamageSystemComponent>(TEXT("DamageSystem"));
}

// Called when the game starts or when spawned
void AAWeekPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AAWeekPlayerController>(GetController());

	/* Initialize UI Manager */
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UIManager = GameInstance->GetSubsystem<UAWeekGameUIManager>();
		UIManager->InitializeUIManager(this);
	}

	PlayerInventoryComponent->OnEncumberedStatusChanged.AddUObject(this, &AAWeekPlayerCharacter::OnEncumbered);
	PlayerInventoryComponent->SelectItemInHotBar(0);

	mAnimInst = Cast<UAWeekPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	/* Initialize crafting component */
	CraftingComponent->InitializeCraftingComponent();
	
	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		const UAWeekGameInput* InputCDO = GetDefault<UAWeekGameInput>();

		Subsystem->AddMappingContext(InputCDO->mContext, 0);
	}

	mWeapon->ChangeWeapon(TEXT("Default"));

	mDamageSystem->OnDamageResponse.AddDynamic(this, &AAWeekPlayerCharacter::OnHit);
	mDamageSystem->OnDeath.AddDynamic(this, &AAWeekPlayerCharacter::Die);
	
}

// Called every frame
void AAWeekPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSprint && mPakour->bCanPakour)
	{
		if (mHunger->IsOnHungerState(EHungerState::Starving) ||
			mStamina->UseStamina(EStaminaUseType::Sprint) == false ||
			GetVelocity().Size() < 50)
		{
			SprintCompleted();
		}
		else
		{
			if (!mHunger->IsOnHungerState(EHungerState::Hungry))
			{
				mPakour->TriggerPakour(EPakourType::Vault);
			}
			mSprintTime += DeltaTime;
		}
	}

	if (mHunger->IsOnHungerState(EHungerState::Healthy))
	{
		mDamageSystem->Execute_Heal(this, 5 * DeltaTime);
	}
	else if (mHunger->IsOnHungerState(EHungerState::Fainting))
	{
		FDamageInfo Info;
		Info.Amount = 10 * DeltaTime;
		mDamageSystem->Execute_TakeDamage(this, Info);
	}

	FHPChangedHandle Msg;
	Msg.HP = mDamageSystem->Execute_GetCurrentHealth(this);
	Msg.MaxHP = mDamageSystem->Execute_GetMaxHealth(this);
	UGameEventMessageSubsystem::Get(this).BroadcastMessage(
		FGameplayTag::RequestGameplayTag(FName("Event.HPChanged")),
		Msg);


	if (bIsCombat)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC && PC->PlayerCameraManager)
		{
			FVector CameraDir = PC->PlayerCameraManager->GetActorForwardVector();
			CameraDir.Z = 0;
			SetActorRotation(CameraDir.Rotation());
		}
	}

	// check interaction period
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

	// update held item ui position
	if (UIManager)
	{
		if (UIManager->GetInventoryController()->IsHoldingItem())
		{
			FVector2D MousePos;
			if (PlayerController->GetMousePosition(MousePos.X, MousePos.Y))
			{
				UIManager->GetInventoryController()->UpdateHeldItemPosition(MousePos);
			}
		}
	}

	if (mAnimInst->GetPlayerMoveState() == EPlayerMoveState::Ledge)
	{
		if (!mStamina->UseStamina(EStaminaUseType::Ledge))
			ClimbEnd();
	}
}

// Called to bind functionality to input
void AAWeekPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (IsValid(EnhancedInput))
	{
		const UAWeekGameInput* InputCDO = GetDefault<UAWeekGameInput>();

		EnhancedInput->BindAction(InputCDO->mMove, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::Move);

		EnhancedInput->BindAction(InputCDO->mLook, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::Look);

		EnhancedInput->BindAction(InputCDO->mJump, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::Jump);

		EnhancedInput->BindAction(InputCDO->mJump, ETriggerEvent::Completed,
			this, &ACharacter::StopJumping);

		EnhancedInput->BindAction(InputCDO->mSprint, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::SprintStart);

		EnhancedInput->BindAction(InputCDO->mSprint, ETriggerEvent::Completed,
			this, &AAWeekPlayerCharacter::SprintCompleted);

		EnhancedInput->BindAction(InputCDO->mAttack, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::Attack);

		EnhancedInput->BindAction(InputCDO->mInteract, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::BeginInteract);

		EnhancedInput->BindAction(InputCDO->mInteract, ETriggerEvent::Completed,
			this, &AAWeekPlayerCharacter::EndInteract);

		EnhancedInput->BindAction(InputCDO->mZoom, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::ZoomHold);
		EnhancedInput->BindAction(InputCDO->mZoom, ETriggerEvent::Completed,
			this, &AAWeekPlayerCharacter::ZoomOut);
		
		EnhancedInput->BindAction(InputCDO->mInventory, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::ToggleInventoryHub);
		EnhancedInput->BindAction(InputCDO->mAttack, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::StartFire);

		EnhancedInput->BindAction(InputCDO->mMainWidget, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::ToggleMainWidget);

		EnhancedInput->BindAction(InputCDO->mBuildingWidget, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::ToggleBuildingWidget);

		EnhancedInput->BindAction(InputCDO->mPreviewRotateL, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::WheelDownPreviewObject);
		EnhancedInput->BindAction(InputCDO->mPreviewRotateR, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::WheelUpPreviewObject);

		EnhancedInput->BindAction(InputCDO->mAttack, ETriggerEvent::Completed,
			this, &AAWeekPlayerCharacter::EndFire);

		EnhancedInput->BindAction(InputCDO->mChangeWeapon, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::ChangeWeapon);

		EnhancedInput->BindAction(InputCDO->mUseItem, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::TakeSomeFood);

		EnhancedInput->BindAction(InputCDO->mTest, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::Die);

		EnhancedInput->BindAction(InputCDO->mCycleHotBar, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::OnMouseWheel);

		EnhancedInput->BindAction(InputCDO->mSelectHotBarSlot, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::OnHotBarKeyPressed);

		// EnhancedInput->BindAction(InputCDO->mCycleHotBar, ETriggerEvent::Started,
		// this, &AAWeekPlayerCharacter::OnLeftClick);
		//
		// EnhancedInput->BindAction(InputCDO->mCycleHotBar, ETriggerEvent::Started,
		// this, &AAWeekPlayerCharacter::OnRightClick);
	}
}

void AAWeekPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (mAnimInst->GetPlayerMoveState() == EPlayerMoveState::Ledge && !mAnimInst->IsPlayingMontageByName(TEXT("Ledge")))
	{
		if (MovementVector.X < 1 && MovementVector.Y > 0 && mAnimInst->GetPlayerMoveState() != EPlayerMoveState::Climb)
		{
			ClimbStart();
		}
		return;
	}
	// input is a Vector2D

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAWeekPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	UAWeekGameUserSettings* Settings = UAWeekGameUserSettings::Get();
	if (Controller != nullptr && Settings)
	{
		
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * Settings->GetMouseSensitivityX());
		AddControllerPitchInput(LookAxisVector.Y * Settings->GetMouseSensitivityY());
	}
}

void AAWeekPlayerCharacter::Jump()
{
	if (mAnimInst->GetPlayerMoveState() == EPlayerMoveState::Ledge && !mAnimInst->IsPlayingMontageByName(TEXT("Ledge")))
	{
		ClimbEnd();
		return;
	}

	if (mPakour->TriggerPakour(EPakourType::Ledge) ||
		GetMovementComponent()->IsFalling() ||
		mAnimInst->IsAnyMontagePlaying())
	{
		return;
	}
	Super::Jump();
}

void AAWeekPlayerCharacter::SetCombatBool(bool Bool)
{
	// Combat True -> Orient False
	GetCharacterMovement()->bOrientRotationToMovement = !Bool;
	bIsCombat = Bool;

	if (mAnimInst)
	{
		if (Bool)
		{
			mAnimInst->SetPlayerWeaponState(EPlayerWeaponState::Aiming);
		}
			
		else
		{
			mAnimInst->SetPlayerWeaponState(EPlayerWeaponState::Default);
		}
			
	}
}

void AAWeekPlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (mAnimInst->IsAnyMontagePlaying())
		return;
	if (mAnimInst->GetCurrentOverride() == FName("Rifle") || mAnimInst->GetCurrentOverride() == FName("Pistol"))
		return;
	SetCombatBool(true);
	mAnimInst->PlayMontageByName(TEXT("Attack"));
	//UE_LOG(LogTemp, Warning, TEXT("%f"), mDamageSystem->GetCurrentHealth_Implementation());

	// Get Weapon Damage from Weapon Component
	// Apply damage later..
}

void AAWeekPlayerCharacter::StartFire()
{
	if (mWeapon->GetWeaponType() != EWeaponType::Ranged)
		return;
	mAnimInst->SetPlayerWeaponState(EPlayerWeaponState::Aiming);
	if (!mWeapon->StartFire())
	{
		StartReload();
	}
}

void AAWeekPlayerCharacter::EndFire()
{
	if (!bIsZooming)
		mAnimInst->SetPlayerWeaponState(EPlayerWeaponState::Default);
	mWeapon->EndFire();
	
}

void AAWeekPlayerCharacter::SprintStart()
{
	if (GetMovementComponent()->IsFalling() || 
		GetVelocity().Size() < 50 || 
		mStamina->GetStamina() < mSprintMinimumStamina ||
		!mPakour->bCanPakour ||
		bIsCombat ||
		mAnimInst->IsAnyMontagePlaying() ||
		mHunger->IsOnHungerState(EHungerState::Starving))
		return;
	GetCharacterMovement()->MaxWalkSpeed *= mSprintSpeedIncRate;
	bSprint = true;
}

void AAWeekPlayerCharacter::SprintCompleted()
{
	GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;

	if (!GetCharacterMovement()->IsFalling() &&
		bSprint &&
		mSprintTime > mRunToStopTime &&
		GetVelocity().Size() >= 50 &&
		mPakour->bCanPakour)
	{
		mAnimInst->PlayMontageByName(TEXT("RunToStop"));
	}

	mSprintTime = 0;
	bSprint = false;
}

void AAWeekPlayerCharacter::ChangeWeapon()
{
	if (mAnimInst->IsAnyMontagePlaying())
		return;

	if (mAnimInst->GetCurrentOverride() == FName("Default"))
	{
		mWeapon->ChangeWeapon(TEXT("Bat"));
		mAnimInst->ChangeAnimOverride(TEXT("Bat"));
	}
	else if (mAnimInst->GetCurrentOverride() == FName("Bat"))
	{
		mWeapon->ChangeWeapon(TEXT("Rifle"));
		mAnimInst->ChangeAnimOverride(TEXT("Rifle"));
	}
	else if (mAnimInst->GetCurrentOverride() == FName("Rifle"))
	{
		mWeapon->ChangeWeapon(TEXT("Pistol"));
		mAnimInst->ChangeAnimOverride(TEXT("Pistol"));
	}
	else
	{
		mWeapon->ChangeWeapon(TEXT("Default"));
		mAnimInst->ChangeAnimOverride(TEXT("Default"));
	}
}

void AAWeekPlayerCharacter::EmptyHand()
{
	mWeapon->ChangeWeapon(TEXT("Default"));
	mAnimInst->ChangeAnimOverride(TEXT("Default"));
}

void AAWeekPlayerCharacter::StartReload()
{
	if (mAnimInst->IsAnyMontagePlaying())
		return;

	mAnimInst->PlayMontageByName(TEXT("Reload"));
}

void AAWeekPlayerCharacter::WeaponReload()
{
	if (mWeapon)
		mWeapon->Reload();
}

void AAWeekPlayerCharacter::TakeSomeFood()
{
	mAnimInst->PlayMontageByName(TEXT("Drink"));
	//GetCharacterMovement()->MaxWalkSpeed *= mBusySpeedDecRate;
}

void AAWeekPlayerCharacter::Heal()
{
	mHunger->ChangeHunger(30.f);
	//GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;
}

void AAWeekPlayerCharacter::VaultStart()
{
	if (GetVelocity().Size() < 50 || GetCharacterMovement()->IsFalling())
		return;

	if (mStamina->UseStamina(EStaminaUseType::Vault))
	{
		mAnimInst->PlayMontageByName(TEXT("Vault"));

		mPakour->bCanPakour = false;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}

void AAWeekPlayerCharacter::VaultEnd()
{
	mPakour->bCanPakour = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AAWeekPlayerCharacter::LedgeStart()
{
	if (!mStamina->UseStamina(EStaminaUseType::LedgeStart))
		return;
	mAnimInst->PlayMontageByName(TEXT("Ledge"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	CameraBoom->bDoCollisionTest = false;
}

void AAWeekPlayerCharacter::LedgeEnd()
{
	mAnimInst->SetPlayerMoveState(EPlayerMoveState::Ledge);
	GetMovementComponent()->StopMovementImmediately();
	CameraBoom->bDoCollisionTest = true;
}

void AAWeekPlayerCharacter::ClimbStart()
{
	mAnimInst->PlayMontageByName(TEXT("Climb"));

	// Motion Warping didnt work... why???????
	// so i move character immediatley
	mAnimInst->SetPlayerMoveState(EPlayerMoveState::Climb);
	CameraBoom->bDoCollisionTest = false;

	FVector Dest = mPakour->GetFirstTopHitLocation();
	Dest.Z+=90;

	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.UUID = 0;
	LatentActionInfo.Linkage = 0;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = FName("ClimbEnd");

	UKismetSystemLibrary::MoveComponentTo(
		GetCapsuleComponent(),
		Dest,
		GetActorRotation(),
		false,
		false,
		1,
		false,
		EMoveComponentAction::Move,
		LatentActionInfo
	);
}

void AAWeekPlayerCharacter::ClimbEnd()
{
	mAnimInst->SetPlayerMoveState(EPlayerMoveState::Ground);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CameraBoom->bDoCollisionTest = true;
}

void AAWeekPlayerCharacter::AttackImpact()
{
	FVector	Forward = GetActorForwardVector();
	FVector	Center = GetActorLocation() + Forward * (25.f + 200 / 2.f);

	TArray<FHitResult>	Result;

	FCollisionQueryParams	param;
	param.AddIgnoredActor(this);
	param.bTraceComplex = false;

	bool Collision = GetWorld()->SweepMultiByChannel(Result, Center, Center,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(100), param);

	DrawDebugSphere(GetWorld(), Center, 100, 20, FColor::Red, false, 0.5f);

	if (Collision)
	{
		for (auto& Hit : Result)
		{
			AActor* HitActor = Hit.GetActor();

			// When hit actor implements DamageAble Interface
			if (HitActor->GetClass()->ImplementsInterface(UDamageAble::StaticClass()))
			{
				FDamageInfo DamageInfo;
				DamageInfo.Amount = mWeapon->GetWeaponDamage();
				DamageInfo.HitInfo.ImpactPoint = Hit.ImpactPoint;
				DamageInfo.HitInfo.ImpactNormal = Hit.ImpactNormal;
				DamageInfo.HitInfo.BoneName = Hit.BoneName;
				DamageInfo.HitInfo.HitComponent = Hit.GetComponent();
				IDamageAble::Execute_TakeDamage(HitActor, DamageInfo);
			}
		}
	}
}

void AAWeekPlayerCharacter::GameOver()
{
	APlayerController* PC = GetController<APlayerController>();
	if (PC)
	{
		// MUST BE CHANGED !!
		DisableInput(PC);
	}
}

void AAWeekPlayerCharacter::FootStepEffect(FName SocketName)
{
	FVector	Position = GetMesh()->GetSocketLocation(SocketName);

	UAISense_Hearing::ReportNoiseEvent(
		GetWorld(),          // World Context
		Position,  // Noise Location
		0.5f,                 // Loudness (0~1)
		this,                // Instigator
		0.f,                 // MaxAge (0 = default)
		TEXT("PlayerFootstep")  // Tag (optional)
	);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FootStepVFX, Position);
}

void AAWeekPlayerCharacter::OnEncumbered(bool bIsEncumbered)
{
	if (bIsEncumbered)
	{
		// TODO: Player get panelty
	}
	else
	{
		// TODO: Player get back to normal
	}
}

// ================================
// INVENTORY SYSTEM
// ================================

void AAWeekPlayerCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart = GetPawnViewLocation();
	FVector TraceEnd(TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance));

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
	if (LookDirection > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UAWeekInteractionInterface::StaticClass()))
			{
				// found new interactable
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				// already interacting
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}
	NoInteractableFound();
}

void AAWeekPlayerCharacter::FoundInteractable(TObjectPtr<AActor> NewInteractable)
{
	UE_LOG(LogTemp, Warning, TEXT("Found Interactable"));

	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	
	UIManager->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	TargetInteractable->BeginFocus();
}

void AAWeekPlayerCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		UIManager->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AAWeekPlayerCharacter::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("begin interact"));
	// verify nothing has changed with the interactable state since beginning interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&AAWeekPlayerCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void AAWeekPlayerCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AAWeekPlayerCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void AAWeekPlayerCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		UIManager->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AAWeekPlayerCharacter::ToggleInventoryHub()
{
	UIManager->ToggleInventoryHub(EAWeekInventoryHubPanel::Crafting);
}

void AAWeekPlayerCharacter::ToggleMainWidget()
{
	UIManager->ToggleMainWidget();
}

void AAWeekPlayerCharacter::ToggleBuildingWidget()
{
	UIManager->ToggleBuildingWidget();
}


void AAWeekPlayerCharacter::WheelDownPreviewObject()
{
	UIManager->PreviewObjectRotateL();	
}

void AAWeekPlayerCharacter::AddHealth(float Delta)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(__FUNCTION__));
	
}

void AAWeekPlayerCharacter::AddStamina(float Delta)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(__FUNCTION__));
}

void AAWeekPlayerCharacter::AddHunger(float Delta)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(__FUNCTION__));
}

void AAWeekPlayerCharacter::OnMouseWheel(const FInputActionValue& Value)
{
	const float WheelAxis = Value.Get<float>();
    
	if (!PlayerInventoryComponent) return;
    
	if (WheelAxis > 0.0f)
	{
		PlayerInventoryComponent->SelectPreviousItemInHotBar();
	}
	else if (WheelAxis < 0.0f)
	{
		PlayerInventoryComponent->SelectNextItemInHotBar();
	}
	PlayerInventoryComponent->SelectCurrentItemInHotBar();
}

void AAWeekPlayerCharacter::OnHotBarKeyPressed(const FInputActionValue& Value)
{
	const float KeyValue = Value.Get<float>();
	const int32 SlotIndex = FMath::RoundToInt(KeyValue) - 1; // 1-9 → 0-8
    
	if (PlayerInventoryComponent)
	{
		PlayerInventoryComponent->SelectItemInHotBar(SlotIndex);
	}
}

void AAWeekPlayerCharacter::SetAnimInstance(FName AnimInstanceName)
{
	mAnimInst->ChangeAnimOverride(AnimInstanceName);
}

void AAWeekPlayerCharacter::WheelUpPreviewObject()
{
	UIManager->PreviewObjectRotateR();
}

void AAWeekPlayerCharacter::ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> InChestInventoryComponent)
{
	ChestInventoryComponent = InChestInventoryComponent;
	UIManager->ToggleInventoryHub(EAWeekInventoryHubPanel::Chest);
}

void AAWeekPlayerCharacter::CloseChestInventory()
{
	UIManager->CloseChestInventory();
}

void AAWeekPlayerCharacter::ToggleCraftingPanel()
{
	UIManager->ToggleInventoryHub(EAWeekInventoryHubPanel::Crafting);
}

void AAWeekPlayerCharacter::CloseCraftingPanel()
{
	UIManager->ToggleInventoryHub(EAWeekInventoryHubPanel::Crafting);
}

