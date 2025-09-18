// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Pakour/AWeekPakourComponent.h"
#include "../Input/AWeekGameInput.h"

#include "AWeek/Interfaces/AWeekInteractionInterface.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/World/AWeekPickupItem.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Player/AWeekPlayerController.h"


DEFINE_LOG_CATEGORY(AWeekPlayerCharacter);

AAWeekPlayerCharacter::AAWeekPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

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
}

// Called when the game starts or when spawned
void AAWeekPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AAWeekPlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		const UAWeekGameInput* InputCDO = GetDefault<UAWeekGameInput>();

		Subsystem->AddMappingContext(InputCDO->mContext, 0);
	}

	mState = Cast<AAWeekPlayerState>(GetPlayerState());
	mAnimInst = Cast<UAWeekPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AAWeekPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSprint && mPakour->bCanPakour)
	{
		if (mState->UseStamina(EStaminaUseType::Sprint) == false || GetVelocity().Size() < 50)
		{
			SprintCompleted();
		}
		else
		{
			mPakour->TriggerPakour();
			mSprintTime += DeltaTime;
		}
	}

	// check interaction period
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
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
	}
}

void AAWeekPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

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
	if (!mAnimInst->IsPlayingRunToStopMontage())
	{
		Super::Jump();
	}
}

void AAWeekPlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (mAnimInst->GetCurrentOverride() == FName("Default"))
		mAnimInst->ChangeAnimOverride(TEXT("Rifle"));
	else
		mAnimInst->ChangeAnimOverride(TEXT("Default"));
}

void AAWeekPlayerCharacter::SprintStart(const FInputActionValue& Value)
{
	if (GetMovementComponent()->IsFalling() || 
		GetVelocity().Size() < 50 || 
		mState->GetStamina() < mSprintMinimumStamina ||
		!mPakour->bCanPakour)
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
		mAnimInst->PlayRunToStopMontage();
	}

	mSprintTime = 0;
	bSprint = false;
}

void AAWeekPlayerCharacter::VaultStart()
{
	if (GetVelocity().Size() < 50 || GetCharacterMovement()->IsFalling())
		return;

	mState->UseStamina(EStaminaUseType::Vault);
	mAnimInst->PlayVaultMontage();
	mPakour->bCanPakour = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void AAWeekPlayerCharacter::VaultEnd()
{
	mPakour->bCanPakour = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}


void AAWeekPlayerCharacter::FootStepEffect(FName SocketName)
{
	FVector	Position = GetMesh()->GetSocketLocation(SocketName);
	UNiagaraSystem* FootStepVFX = LoadObject<UNiagaraSystem>(GetWorld(), TEXT("/Script/Niagara.NiagaraSystem'/Game/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_General1_Surface.PSN_General1_Surface'"));
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

	
	PlayerController->UpdateInteractionWidget(&TargetInteractable->InteractableData);
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

		PlayerController->HideInteractionWidget();

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
		PlayerController->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AAWeekPlayerCharacter::ToggleMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Toggle Menu"));
	PlayerController->ToggleMainPanel();
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
	PlayerController->ToggleChestInventory(ChestInventory);
}

//void AAWeekPlayerCharacter::OpenChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
//{
//	PlayerController->ActivateChestInventory(ChestInventory);
//}

void AAWeekPlayerCharacter::CloseChestInventory()
{
	PlayerController->DeactivateChestInventory();
}
