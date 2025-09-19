// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Pakour/AWeekPakourComponent.h"
#include "Stamina/AWeekStaminaComponent.h"
#include "../Player/Weapon/AWeekWeaponComponent.h"
#include "../System/DamageSystemComponent.h"
#include "../Input/AWeekGameInput.h"
#include "../System/DaySystem/AWeekDaySystem.h"

#include "AWeek/Interfaces/AWeekInteractionInterface.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/World/AWeekPickupItem.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Player/AWeekUIController.h"


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

	GetMesh()->SetCollisionProfileName(TEXT("Player"));

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

	PlayerInventory = CreateDefaultSubobject<UAWeekInventoryComponent>(TEXT("PlayerInventory"));

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 250.0f;
	mStamina = CreateDefaultSubobject<UAWeekStaminaComponent>(TEXT("Stamina"));
	mWeapon = CreateDefaultSubobject<UAWeekWeaponComponent>(TEXT("Weapon"));
	mDamageSystem = CreateDefaultSubobject<UDamageSystemComponent>(TEXT("DamageSystem"));
}

// Called when the game starts or when spawned
void AAWeekPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UIController = Cast<AAWeekUIController>(GetController());

	if (IsValid(UIController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(UIController->GetLocalPlayer());

		const UAWeekGameInput* InputCDO = GetDefault<UAWeekGameInput>();

		Subsystem->AddMappingContext(InputCDO->mContext, 0);
	}

	mAnimInst = Cast<UAWeekPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	mWeapon->ChangeWeapon(TEXT("Default"));

	mDamageSystem->OnDeath.AddDynamic(this, &AAWeekPlayerCharacter::Die);
}

// Called every frame
void AAWeekPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSprint && mPakour->bCanPakour)
	{
		if (mStamina->UseStamina(EStaminaUseType::Sprint) == false || GetVelocity().Size() < 50)
		{
			SprintCompleted();
		}
		else
		{
			mPakour->TriggerPakour(EPakourType::Vault);
			mSprintTime += DeltaTime;
		}
	}

	if (bIsCombat)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC && PC->PlayerCameraManager)
		{
			FVector CameraLoc = PC->PlayerCameraManager->GetCameraLocation();
			FVector CameraDir = PC->PlayerCameraManager->GetActorForwardVector();

			FVector TraceEnd = CameraLoc + CameraDir * 10000.f;

			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			FVector TargetPoint = TraceEnd;
			if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLoc, TraceEnd, ECC_Visibility, Params))
			{
				TargetPoint = Hit.Location;
			}

			FVector ToTarget = TargetPoint - GetActorLocation();
			ToTarget.Z = 0;

			FRotator TargetRot = ToTarget.Rotation();
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, 10.f));
		}
	}

	// check interaction period
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
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
		
		EnhancedInput->BindAction(InputCDO->mInventory, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::ToggleMenu);
		EnhancedInput->BindAction(InputCDO->mAttack, ETriggerEvent::Triggered,
			this, &AAWeekPlayerCharacter::Fire);


		EnhancedInput->BindAction(InputCDO->mAttack, ETriggerEvent::Completed,
			this, &AAWeekPlayerCharacter::EndFire);

		EnhancedInput->BindAction(InputCDO->mChangeWeapon, ETriggerEvent::Started,
			this, &AAWeekPlayerCharacter::ChangeWeapon);
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

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
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

void AAWeekPlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (mAnimInst->IsAnyMontagePlaying())
		return;
	if (mAnimInst->GetCurrentOverride() == FName("Rifle"))
		return;
	SetCombatBool(true);
	mAnimInst->PlayMontageByName(TEXT("Attack"));

	// Get Weapon Damage from Weapon Component
	// Apply damage later..
}

void AAWeekPlayerCharacter::Fire()
{
	if (mAnimInst->GetCurrentOverride() != FName("Rifle"))
		return;

	GetCharacterMovement()->MaxWalkSpeed = mFiringSpeed;

	if (mAnimInst->GetPlayerWeaponState() == EPlayerWeaponState::Default)
	{
		mAnimInst->SetPlayerWeaponState(EPlayerWeaponState::Gun);
		mAnimInst->PlayMontageByName(TEXT("Fire"));
		SetCombatBool(true);
	}

	//mAnimInst->ChangeAnimOverride(TEXT("Rifle_Firing"));
}

void AAWeekPlayerCharacter::EndFire()
{
	if (mAnimInst->GetCurrentOverride() != FName("Rifle"))
		return;

	GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;
	mAnimInst->SetPlayerWeaponState(EPlayerWeaponState::Default);
	mAnimInst->StopMontageByName(TEXT("Fire"));
	SetCombatBool(false);

	//mAnimInst->ChangeAnimOverride(TEXT("Rifle"));
}

void AAWeekPlayerCharacter::SprintStart()
{
	if (GetMovementComponent()->IsFalling() || 
		GetVelocity().Size() < 50 || 
		mStamina->GetStamina() < mSprintMinimumStamina ||
		!mPakour->bCanPakour ||
		mAnimInst->GetPlayerWeaponState()==EPlayerWeaponState::Gun)
		return;
	GetCharacterMovement()->MaxWalkSpeed = mSprintSpeed;
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
	else
	{
		mWeapon->ChangeWeapon(TEXT("Default"));
		mAnimInst->ChangeAnimOverride(TEXT("Default"));
	}
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
	mAnimInst->SetPlayerMoveState(EPlayerMoveState::Ledge);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	CameraBoom->bDoCollisionTest = false;
}

void AAWeekPlayerCharacter::LedgeEnd()
{
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
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeBox(FVector(100)), param);

	DrawDebugSphere(
		GetWorld(),
		Center,             // �߽�
		100.f,              // ������ (MakeSphere���� �� ��)
		16,                 // ���׸�Ʈ (������ ����)
		FColor::Green,      // ��
		false,              // ���� ���� (true�� ���� ����)
		1.0f                // ���� �ð� (1��)
	);


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
				bool bDamaged = IDamageAble::Execute_TakeDamage(HitActor, DamageInfo);
				if (bDamaged)
					UE_LOG(LogTemp, Warning, TEXT("Hit Total Target: %d\t Damage Amount: %f"), Result.Num(), DamageInfo.Amount);
			}
		}
	}
}

void AAWeekPlayerCharacter::FireBullet()
{
	FVector	MuzzleLoctaion = mWeapon->GetWeaponMuzzle();

	if (FireEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			FireEffect,
			MuzzleLoctaion,
			GetActorRotation(),
			FVector(1.0f)
		);
	}

	if (FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, MuzzleLoctaion);
	}
	
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//AASeedTestBullet* Bullet = GetWorld()->SpawnActor<AASeedTestBullet>(MuzzleLoctaion, GetActorRotation(), Param);
}

void AAWeekPlayerCharacter::Die()
{
	mAnimInst->PlayMontageByName(TEXT("Die"));
}


void AAWeekPlayerCharacter::FootStepEffect(FName SocketName)
{
	FVector	Position = GetMesh()->GetSocketLocation(SocketName);
	UNiagaraSystem* FootStepVFX = LoadObject<UNiagaraSystem>(GetWorld(), TEXT("/Script/Niagara.NiagaraSystem'/Game/ThirdParty/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_General1_Surface.PSN_General1_Surface'"));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FootStepVFX, Position);
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

	
	UIController->UpdateInteractionWidget(&TargetInteractable->InteractableData);
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

		UIController->HideInteractionWidget();

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
		UIController->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AAWeekPlayerCharacter::ToggleMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Toggle Menu"));
	UIController->ToggleMainPanel();
}

void AAWeekPlayerCharacter::DropItemFromItemSlot(const FAWeekItemSlot& ItemSlot, const int32 QuantityToDrop)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 50.0f;
	const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
	UAWeekItemBase* ItemToDrop = ItemSlot.Item;

	//const int32 RemoveQuantity = PlayerInventory->RemoveAmountOfItem(ItemSlot, QuantityToDrop);
	const int32 RemoveQuantity = ItemSlot.OwningInventory->RemoveAmountOfItem(ItemSlot.ItemSlotIndex, QuantityToDrop);

	AAWeekPickupItem* Pickup = GetWorld()->SpawnActor<AAWeekPickupItem>(AAWeekPickupItem::StaticClass(), SpawnTransform, SpawnParams);

	Pickup->InitializeDrop(ItemToDrop, RemoveQuantity);
}

void AAWeekPlayerCharacter::ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
{
	UIController->ToggleChestInventory(ChestInventory);
}

//void AAWeekPlayerCharacter::OpenChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
//{
//	UIController->ActivateChestInventory(ChestInventory);
//}

void AAWeekPlayerCharacter::CloseChestInventory()
{
	UIController->DeactivateChestInventory();
}
