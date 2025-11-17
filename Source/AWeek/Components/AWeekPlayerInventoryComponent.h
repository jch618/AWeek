// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeekPlayerInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHotbarSelectionChanged, int32 PreviousIndex, int32 NewIndex);
class UAWeekItemTrashCanSlot;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekPlayerInventoryComponent : public UAWeekInventoryComponent
{
	GENERATED_BODY()

public:
	FOnHotbarSelectionChanged OnHotbarSelectionChanged;
	
	UAWeekPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	/* Hot Bar Function */
	void UseSelectedItemPrimary(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter);
	void UseSelectedItemSecondary(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter);
	
	void SelectItemInHotBar(const int32 InHotBarIndex);
	void SelectNextItemInHotBar();
	void SelectPreviousItemInHotBar();
	void SelectCurrentItemInHotBar();
	
	FORCEINLINE int32 GetHotBarInventorySize() const { return HotBarInventorySize; }
	FORCEINLINE int32 GetTrashCanSlotIndex() const { return TrashCanSlotIndex; }
	FORCEINLINE const FAWeekInventorySlotData& GetHotBarItemAt(const int32 HotBarIndex) const
		{ return InventoryContents[HotBarStartIndex + HotBarIndex % HotBarInventorySize]; }  
	/* Trash Can Function */
	void SetTrashCanSlot(const TObjectPtr<UAWeekItemBase> InItem);
	void ClearTrashCanSlot();

protected:
	virtual void AddNewItem(UAWeekItemBase* Item, const int32 AmountToAdd, int32 TargetIndex = -1) override;
	virtual void ClearItemSlot(FAWeekInventorySlotData& ItemSlotToRemove) override;
private:
	FORCEINLINE bool IsHotBarSlotIndex(const int32 Index) const { return Index >= HotBarStartIndex && Index < HotBarStartIndex + HotBarInventorySize; }

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 HotBarInventorySize;
	
	int32 HotBarCurrentIndex;
	int32 HotBarPreviousIndex;
	int32 HotBarStartIndex;
	int32 TrashCanSlotIndex;
};
