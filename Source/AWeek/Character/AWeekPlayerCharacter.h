// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

#include "AWeekCharacter.h"
#include "../Player/AWeekPlayerAnimInstance.h"
#include "AWeek/Interfaces/AWeekInteractionInterface.h"

#include "../System/GameEventMessageSubsystem.h"
#include "../System/DamageInfo.h"
#include "../System/IDamageAble.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Kismet/KismetSystemLibrary.h"

#include "AWeekPlayerCharacter.generated.h"


class AAWeekPlayerController;
class UAWeekCraftingComponent;
class UAWeekGameUIManager;
class UAWeekInventoryComponent;
class UAWeekItemBase;
struct FAWeekInventorySlotData;

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
class AWEEK_API AAWeekPlayerCharacter : public AAWeekCharacter, public IDamageAble
{
	GENERATED_BODY()
public:
	AAWeekPlayerCharacter();

protected:
	/*--------------CAMERA--------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/*--------------ANIMINST--------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAWeekPlayerAnimInstance> mAnimInst;

	/*--------------PAKOUR--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekPakourComponent> mPakour;

	/*--------------STAMINA--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekStaminaComponent> mStamina;
	
	/*--------------WEAPON--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekWeaponComponent> mWeapon;

	/*--------------WEAPON--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekHungerComponent> mHunger;

	/*--------------DAMAGE--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDamageSystemComponent> mDamageSystem;

	/*--------------SOUNDS--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;

	/*--------------EVENT--------------*/
	FGameEventMessageListenerHandle DayChangedHandle;

	/*--------------VARIABLES--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsZooming = false;

	UPROPERTY(EditAnywhere)
	float mWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	float mFiringSpeed = 200.f;

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
	float mSprintStaminaUsage = 25; // �޸��� �ʴ� ���¹̳� �Ҹ�

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mVaultStaminaUsage = 20;


	// =====================================================
	// INVENTORY SYSTEM
	// ====================================================
	UPROPERTY()
	TObjectPtr<UAWeekGameUIManager> UIManager;
	
	UPROPERTY()
	TObjectPtr<AAWeekPlayerController> PlayerController;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IAWeekInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	TObjectPtr<UAWeekInventoryComponent> PlayerInventory;

	UPROPERTY(VisibleAnywhere, Category = "Character | Crafting")
	TObjectPtr<UAWeekCraftingComponent> CraftingComponent;
	
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
	void ZoomHold()
	{
		SetCombatBool(true);
		bIsZooming = true;
	}
	void ZoomOut()
	{
		SetCombatBool(false);
		bIsZooming = false;
	}
	void StartFire();
	void EndFire();
	void SprintStart();
	void SprintCompleted();
	void ChangeWeapon();
	void StartReload();
	void WeaponReload();

	UFUNCTION()
	virtual void ClimbEnd();
	void AttackImpact();

	UFUNCTION()
	void OnHit(EDamageResponse Response)
	{
		UE_LOG(LogTemp, Warning, TEXT("OUCH!"));
	}

	UFUNCTION()
	void Die();

	virtual void VaultStart();
	virtual void VaultEnd();
	virtual void LedgeStart();
	virtual void LedgeEnd();
	virtual void ClimbStart();

	UFUNCTION(BlueprintCallable)
	void SetCombatBool(bool Bool);

	//bool TakeDamage_Implementation(EDamageResponse DamageResponse);

	// =====================================================
	// INVENTORY SYSTEM
	// =====================================================
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }
	FORCEINLINE TObjectPtr<UAWeekInventoryComponent> GetInventory() const { return PlayerInventory; }
	void UpdateInteractionWidget() const;
	void ToggleInventoryMainPanel();
	void DropItemFromItemSlot(const FAWeekInventorySlotData& ItemSlot, const int32 QuantityToDrop);
	void ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);
	//void OpenChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);
	void CloseChestInventory();
	void ToggleCraftingMainPanel();
	void CloseCraftingMainPanel();

public:
	// =====================================================
	// INVENTORY SYSTEM
	// =====================================================
	void PerformInteractionCheck();
	void FoundInteractable(TObjectPtr<AActor> NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();


public:
	UFUNCTION(BlueprintCallable)
	void FootStepEffect(FName SocketName);
};
