// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

#include "AWeekCharacter.h"
#include "../Player/AWeekPlayerState.h"
#include "../Player/AWeekPlayerAnimInstance.h"
#include "AWeek/Interfaces/AWeekInteractionInterface.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "AWeekPlayerCharacter.generated.h"


class UAWeekInventoryComponent;
class UAWeekItemBase;
class AAWeekPlayerController;
struct FAWeekItemSlot;

USTRUCT()
struct FAWeekInteractionData
{
	GENERATED_BODY()

	FAWeekInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f) {}

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

DECLARE_LOG_CATEGORY_EXTERN(AWeekPlayerCharacter, Warning, All);

UCLASS()
class AWEEK_API AAWeekPlayerCharacter : public AAWeekCharacter
{
	GENERATED_BODY()
public:
	AAWeekPlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	TObjectPtr<AAWeekPlayerState> mState;
	TObjectPtr<UAWeekPlayerAnimInstance> mAnimInst;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekPakourComponent> mPakour;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSprint = false;

	UPROPERTY(EditAnywhere)
	float mWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	float mSprintSpeed = 500.f;

	UPROPERTY(EditAnywhere)
	float mRunToStopTime = 0.75f;

	UPROPERTY(EditAnywhere)
	float mJumpVelocity = 550.f;

	UPROPERTY(EditAnywhere)
	float mSprintMinimumStamina = 25.f;

	UPROPERTY(VisibleAnywhere)
	float mSprintTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mSprintStaminaUsage = 25; // 달리기 초당 스태미나 소모량

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mVaultStaminaUsage = 20; 

	// =====================================================
	// INVENTORY SYSTEM
	// ====================================================
	UPROPERTY()
	TObjectPtr<AAWeekPlayerController> PlayerController;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IAWeekInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	TObjectPtr<UAWeekInventoryComponent> PlayerInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float InteractionCheckFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionCheckDistance;

	UPROPERTY()
	FTimerHandle TimerHandle_Interaction;

	UPROPERTY()
	FAWeekInteractionData InteractionData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Attack(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprintCompleted();


	// =====================================================
	// INVENTORY SYSTEM
	// =====================================================
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }
	FORCEINLINE TObjectPtr<UAWeekInventoryComponent> GetInventory() const { return PlayerInventory; }
	void UpdateInteractionWidget() const;
	void DropItemFromItemSlot(const FAWeekItemSlot& ItemSlot, const int32 QuantityToDrop);
	void ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);
	//void OpenChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);
	void CloseChestInventory();

protected:
	virtual void VaultStart();
	virtual void VaultEnd();

	// =====================================================
	// INVENTORY SYSTEM
	// =====================================================
	void PerformInteractionCheck();
	void FoundInteractable(TObjectPtr<AActor> NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

	void ToggleMenu();

public:
	UFUNCTION(BlueprintCallable)
	void FootStepEffect(FName SocketName);
};
