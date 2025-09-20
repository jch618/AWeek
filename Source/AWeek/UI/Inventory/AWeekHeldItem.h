// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekHeldItem.generated.h"

class UAWeekHeldItemVisual;
class UAWeekInventoryComponent;
class UAWeekItemBase;
class UAWeekHeldItemVisual;

/**
 * 
 */
USTRUCT()
struct FHeldItemData
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAWeekHeldItemVisual> HeldItemVisual;

	UPROPERTY()
	TObjectPtr<UAWeekItemBase> ItemReference;

	UPROPERTY()
	TObjectPtr<UAWeekInventoryComponent> SourceInventory;

	int32 SourceSlotIndex = -1;
};

UCLASS()
class AWEEK_API UAWeekHeldItem : public UObject
{
	GENERATED_BODY()

public:
	UAWeekHeldItem();
	
	FORCEINLINE bool IsEmpty() const { return HeldItemData.ItemReference == nullptr; }
	FORCEINLINE const UAWeekInventoryComponent* GetSourceInventory() const { return HeldItemData.SourceInventory; }
	FORCEINLINE const UAWeekItemBase* GetItemReference() const { return HeldItemData.ItemReference; }
	void SetHeldItemQuantity(int32 Quantity);
	void SetItemReference(TObjectPtr<UAWeekItemBase> NewItemReference);
	UAWeekItemBase* ReleaseHeldItem();
	void InitializeHeldItem(const FHeldItemData& NewHeldItemData);
	void UpdateHeldVisualPosition(FVector2D MousePos);
	void ClearHeldItem();
	void ReturnHeldItemToInventory();
protected:
	float HeldItemVisualXOffset;
	float HeldItemVisualYOffset;
	
	FHeldItemData HeldItemData;

	void UpdateHeldItem();
};
