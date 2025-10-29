// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Crafting/AWeekCraftingItemSlot.h"

#include "AWeek/Data/AWeekItemDataStructs.h"
#include "Components/Image.h"

void UAWeekCraftingItemSlot::InitializeCraftingItemSlot(int32 InRecipeIndex, const FAWeekItemData& InItemData, int32 InItemQuantity, bool bInCraftable)
{
	Super::InitializeItemSlot(InItemData, InItemQuantity);

	RecipeIndex = InRecipeIndex;
	bIsCraftable = bInCraftable;
	if (bIsCraftable)
	{
		ItemIcon->SetBrushTintColor(FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
		ItemIcon->SetBrushTintColor(FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

FReply UAWeekCraftingItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnCraftingSlotLeftClicked.Broadcast(RecipeIndex, bIsCraftable);
		return FReply::Handled();
	}
	return Reply.Unhandled();
}
