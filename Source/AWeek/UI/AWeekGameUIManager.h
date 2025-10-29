// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AWeekGameUIManager.generated.h"

class UAWeekCraftingComponent;
class UAWeekCraftingMainPanel;
class UAWeekUIDataAsset;
class AAWeekPlayerController;
class UAWeekInventoryMainPanel;
class UMainUIWidget;
class UAWeekInteractionWidget;
struct FAWeekInteractableData;
class UAWeekInventoryComponent;
class UAWeekHeldItemVisual;
class UAWeekHeldItem;
class UAWeekItemBase;
struct FAWeekInventorySlotData;
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
	
	void ToggleInventoryMainPanel();
	void ToggleMainWidget();
	void ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);

	void ToggleCraftingMainPanel(TObjectPtr<UAWeekCraftingComponent> CraftingComponent, const TObjectPtr<UAWeekInventoryComponent> InventoryComponent);
	// void ShowCrosshair();
	// void HideCrosshair();

	void ShowInteractionWidget();
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FAWeekInteractableData* InteractableData);

	void ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory) const;
	void DeactivateChestInventory();

	void HideCraftingMainPanel();
	
	// held item functions
	FORCEINLINE bool IsHoldingItem() const;
	FORCEINLINE void SetHeldItem(TObjectPtr<UAWeekHeldItem> NewHeldItem) { HeldItem = NewHeldItem; }
	void UpdateHeldItemPosition(FVector2D NewPosition);

	// inventory slot delegates
	void HandleItemSlotLeftClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void HandleItemSlotRightClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void HandleItemSlotShiftLeftClick(const FAWeekInventorySlotData& ClickedItemSlot) const;
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

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAWeekCraftingMainPanel> CraftingMainPanelClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;
	UPROPERTY()
	TObjectPtr<UMainUIWidget> MainUIWidget;
	
	UPROPERTY()
	TObjectPtr<UAWeekInventoryMainPanel> InventoryMainPanelWidget;

	UPROPERTY()
	TObjectPtr<UAWeekCraftingMainPanel> CraftingMainPanelWidget;;
	
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

	void ShowInventoryMainPanel();
	void ShowMainWidget();
	
	void HideInventoryMainPanel();
	void HideMainWidget();
	void ShowCraftingMainPanel();

	void ShowCraftingMainPanel(TObjectPtr<UAWeekCraftingComponent> CraftingComponent, const TObjectPtr<UAWeekInventoryComponent> InventoryComponent);

private:
	UPROPERTY(EditAnywhere, Category = "UI Settings")
	FSoftObjectPath UIDataAssetPath;
    
	UPROPERTY()
	UAWeekUIDataAsset* UIDataAsset;
};