// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeekUIController.generated.h"

class UAWeekInventoryMainPanel;
class UAWeekInteractionWidget;
struct FAWeekInteractableData;
class UAWeekInventoryComponent;
class UAWeekDragItemVisual;
class UAWeekHeldItem;
class UAWeekItemBase;
struct FAWeekItemSlot;

/**
 * 
 */
UCLASS()
class AWEEK_API AAWeekUIController : public AAWeekPlayerController
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
	void ShowMainPanel();
	void HideMainPanel();
	void ToggleMainPanel();
	void ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);

	void ShowCrosshair();
	void HideCrosshair();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FAWeekInteractableData* InteracableData);

	void ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);
	void DeactivateChestInventory();

	// held item functions
	FORCEINLINE bool IsHoldingItem() const;
	FORCEINLINE void SetHeldItem(TObjectPtr<UAWeekHeldItem> NewHeldItem) { HeldItem = NewHeldItem; }

	void HandleItemSlotLeftClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void HandleItemSlotRightClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);

	void HandleItemSlotShiftLeftClick(const FAWeekItemSlot& ClickedItemSlot);
protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAWeekDragItemVisual> HeldItemVisualClass;

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
	TObjectPtr<UAWeekHeldItem> HeldItem;

	//================================================================
	//	FUNCTIONS
	//================================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void MergeItem(int32 TargetSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void CreateHeldItem(TObjectPtr<UAWeekItemBase> NewNeldItem, TObjectPtr<UAWeekInventoryComponent> SourceInventory, int32 SourceItemSlotIndex);
};
