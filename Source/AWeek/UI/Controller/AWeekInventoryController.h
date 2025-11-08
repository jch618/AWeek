// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AWeekInventoryController.generated.h"

struct FAWeekInventorySlotData;
class UAWeekInventoryComponent;
class UAWeekGameUIManager;
class UAWeekHeldItem;
class UAWeekHeldItemVisual;
class UAWeekItemBase;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AWEEK_API UAWeekInventoryController : public UObject
{
	GENERATED_BODY()

public:
	//================================================================
	//	FUNCTIONS
	//================================================================
	void InitializeInventoryController(TObjectPtr<UAWeekGameUIManager> InUIManager, TSubclassOf<UAWeekHeldItemVisual> InHeldItemVisualClass);
	
	// held item functions
	FORCEINLINE bool IsHoldingItem() const;
	FORCEINLINE UAWeekHeldItem* GetHeldItem() const { return HeldItem; };
	FORCEINLINE void SetHeldItem(const TObjectPtr<UAWeekHeldItem> InHeldItem) { HeldItem = InHeldItem; }
	void UpdateHeldItemPosition(FVector2D NewPosition);
	void ReturnHeldItemToInventory();

	// inventory slot delegates
	void HandleItemSlotLeftClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void HandleItemSlotRightClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void HandleItemSlotShiftLeftClick(const FAWeekInventorySlotData& ClickedItemSlot) const;
	
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================

	
protected:
	//================================================================
	//	FUNCTIONS
	//================================================================
	
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================


private:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY()
	TObjectPtr<UAWeekGameUIManager> UIManager;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UAWeekHeldItemVisual> HeldItemVisualClass;

	UPROPERTY()
	TObjectPtr<UAWeekHeldItem> HeldItem;
	
	//================================================================
	//	FUNCTIONS
	//================================================================
	void MergeItem(int32 TargetSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
	void CreateHeldItem(TObjectPtr<UAWeekItemBase> InHeldItem, TObjectPtr<UAWeekInventoryComponent> SourceInventory, int32 SourceItemSlotIndex);

};
