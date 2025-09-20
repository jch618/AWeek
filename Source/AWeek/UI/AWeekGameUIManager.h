// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AWeekGameUIManager.generated.h"

class UAWeekUIDataAsset;
class AAWeekPlayerController;
class UAWeekInventoryMainPanel;
class UAWeekInteractionWidget;
struct FAWeekInteractableData;
class UAWeekInventoryComponent;
class UAWeekHeldItemVisual;
class UAWeekHeldItem;
class UAWeekItemBase;
struct FAWeekItemSlot;
class AAWeekPlayerCharacter;

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekGameUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	// =====================================================
	// INVENTORY SYSTEM
	// =====================================================

	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================


	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//TSubclassOf<UUserWidget> CrosshairWidgetClass;

	//================================================================
	//	FUNCTIONS
	//================================================================

public:
	UAWeekGameUIManager();
	void InitializeUIManager();

	void ShowMainPanel();
	void HideMainPanel();
	void ToggleMainPanel();
	void ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);

	// void ShowCrosshair();
	// void HideCrosshair();

	void ShowInteractionWidget();
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FAWeekInteractableData* InteractableData);

	void ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory) const;
	void DeactivateChestInventory();

	// held item functions
	FORCEINLINE bool IsHoldingItem() const;
	FORCEINLINE void SetHeldItem(TObjectPtr<UAWeekHeldItem> NewHeldItem) { HeldItem = NewHeldItem; }

	void UpdateHeldItemPosition(FVector2D NewPosition);

	void HandleItemSlotLeftClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void HandleItemSlotRightClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);

	void HandleItemSlotShiftLeftClick(const FAWeekItemSlot& ClickedItemSlot) const;
protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAWeekHeldItemVisual> HeldItemVisualClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAWeekInventoryMainPanel> InventoryMainPanelClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAWeekInteractionWidget> InteractionWidgetClass;

	UPROPERTY()
	TObjectPtr<UAWeekInventoryMainPanel> InventoryMainPanelWidget;

	UPROPERTY()
	TObjectPtr<UAWeekInteractionWidget> InteractionWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> CrosshairWidget;

	UPROPERTY()
	TObjectPtr<ULocalPlayer> LocalPlayer;

	UPROPERTY()
	TObjectPtr<AAWeekPlayerController> PlayerController;

	UPROPERTY()
	TObjectPtr<UAWeekHeldItem> HeldItem;

	//================================================================
	//	FUNCTIONS
	//================================================================
	void MergeItem(int32 TargetSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void CreateHeldItem(TObjectPtr<UAWeekItemBase> NewHeldItem, TObjectPtr<UAWeekInventoryComponent> SourceInventory, int32 SourceItemSlotIndex);

private:
	UPROPERTY(EditAnywhere, Category = "UI Settings")
	FSoftObjectPath UIDataAssetPath;
    
	UPROPERTY()
	UAWeekUIDataAsset* UIDataAsset;
};