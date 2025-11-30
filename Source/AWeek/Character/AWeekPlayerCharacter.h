// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

#include "AWeekCharacter.h"
#include "../Player/AWeekPlayerAnimInstance.h"
#include "AWeek/Interfaces/AWeekInteractionInterface.h"

#include "../UI/InGame/Player/AWeekPlayerStateWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "../System/DamageInfo.h"
#include "../System/IDamageAble.h"

#include "InputActionValue.h"
#include "AWeekPlayerCharacter.generated.h"


class AAWeekLootChest;
class UAWeekPlayerInventoryComponent;
enum class EAWeekInventoryHubPanel : uint8;
class UAWeekLootComponent;
class AAWeekPlayerController;
class UAWeekCraftingComponent;
class UAWeekGameUIManager;
class UAWeekInventoryComponent;
class UAWeekItemBase;
struct FAWeekInventorySlotData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractionTargetChanged, const FAWeekInteractableData& InteractableData)

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

	FOnInteractionTargetChanged OnInteractionTargetChanged;
	
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

	/*--------------HUNGER--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekHungerComponent> mHunger;
	UPROPERTY(EditAnywhere)
	float HPGenRatio = 0.5f;
	UPROPERTY(EditAnywhere)
	float HPPenaltiyRatio = 1;

	/*--------------SOUND--------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAWeekSoundComponent> mSound;

	/*--------------HUD--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAWeekPlayerStateWidget> mStateWidget;

	/*--------------DAMAGE--------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDamageSystemComponent> mDamageSystem;

	/*--------------SOUNDS--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;

	/*--------------VARIABLES--------------*/
	UPROPERTY(VisibleAnywhere)
	bool bGameOver = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsZooming = false;

	UPROPERTY(EditAnywhere)
	float mBaseWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	float mWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	float mBusySpeedDecRate = 0.5f;

	UPROPERTY(EditAnywhere)
	float mZoomSpeedDecRate = 0.8f;

	UPROPERTY(EditAnywhere)
	float mSprintSpeedIncRate = 1.5f;

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
	// INVENTORY
	// ====================================================
	UPROPERTY()
	TObjectPtr<UAWeekGameUIManager> UIManager;
	
	
	UPROPERTY()
	TObjectPtr<AAWeekPlayerController> PlayerController;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IAWeekInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	TObjectPtr<UAWeekPlayerInventoryComponent> PlayerInventoryComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	TObjectPtr<UAWeekInventoryComponent> ChestInventoryComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Character | Crafting")
	TObjectPtr<UAWeekCraftingComponent> CraftingComponent;

	// =====================================================
	// INTERACTION
	// ====================================================
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
	UFUNCTION(BlueprintCallable)
	void EndFire();
	void SprintStart();
	void SprintCompleted();
	void ChangeWeapon();
	void EmptyHand();
	void StartReload();
	void WeaponReload();

	void SetDefaultWalkSpeed(float Speed)
	{
		mWalkSpeed = Speed;
		GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;
	}

	float GetBaseWalkSpeed()
	{
		return mBaseWalkSpeed;
	}
	
	void TakeSomeFood(); // pair with heal
	void Heal();

	UFUNCTION()
	virtual void ClimbEnd();
	void AttackImpact();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHit(EDamageResponse Response);

	UFUNCTION(BlueprintImplementableEvent)
	void Die();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();

	virtual void VaultStart();
	virtual void VaultEnd();
	virtual void ClimbStart();

	UFUNCTION(BlueprintCallable)
	void SetCombatBool(bool Bool);

	//bool TakeDamage_Implementation(EDamageResponse DamageResponse);

	// =====================================================
	// INVENTORY SYSTEM
	// =====================================================
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }
	FORCEINLINE UAWeekPlayerInventoryComponent* GetPlayerInventoryComponent() const { return PlayerInventoryComponent; }
	FORCEINLINE UAWeekCraftingComponent* GetCraftingComponent() const { return CraftingComponent; }
	void UpdateInteractionWidget() const;
	
	void ToggleInventoryHub();
	void ToggleMainWidget();
	void ToggleBuildingWidget();
	
	// void DropItemFromItemSlot(const FAWeekInventorySlotData& ItemSlot, const int32 QuantityToDrop);
	void ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> InChestInventoryComponent);
	//void OpenChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);
	void CloseChestInventory();
	void ToggleCraftingPanel();
	void CloseCraftingPanel();

	FORCEINLINE void SetChestInventoryComponent(const TObjectPtr<UAWeekInventoryComponent> InChestInventoryComponent)
	{
		ChestInventoryComponent = InChestInventoryComponent;
	}
	FORCEINLINE UAWeekInventoryComponent* GetChestInventoryComponent() const { return ChestInventoryComponent;}

	void AddHealth(float Delta);
	void AddStamina(float Delta);
	void AddHunger(float Delta);

	/* Hot bar control */
	void OnMouseWheel(const FInputActionValue& Value);
	void OnHotBarKeyPressed(const FInputActionValue& Value);

	/* temporary test */

	FORCEINLINE UAWeekPlayerAnimInstance* GetAnimInstance() { return mAnimInst; }
	FORCEINLINE UAWeekWeaponComponent* GetWeaponComponent() const { return mWeapon; }
	
	void WheelUpPreviewObject();
	void WheelDownPreviewObject();

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
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* FootStepVFX;
	UFUNCTION(BlueprintCallable)
	void FootStepEffect(FName SocketName);

private:
	/* When inventory capacity exceed, player character takes debuff */
	UFUNCTION(Category = "Inventory")
	void OnEncumbered(bool bIsEncumbered);
};
