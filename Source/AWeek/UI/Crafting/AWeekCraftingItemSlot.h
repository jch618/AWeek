// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/Inventory/AWeekItemSlot.h"
#include "AWeekCraftingItemSlot.generated.h"

/**
 * 
 */

UCLASS()
class AWEEK_API UAWeekCraftingItemSlot : public UAWeekItemSlot
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCraftingSlotLeftClicked, int32 ClickedCraftingSlotIndex)
	FOnCraftingSlotLeftClicked OnCraftingSlotLeftClicked;
	
	FORCEINLINE void SetIsCraftable(bool bNewIsCraftable) { bIsCraftable = bNewIsCraftable; }

	void InitializeCraftingItemSlot(int32 InRecipeIndex, const FAWeekItemData& InItemData, int32 InItemQuantity, bool bInCraftable);
protected:
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	int32 RecipeIndex;

	bool bIsCraftable;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
