// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Crafting/AWeekCraftingDetailPanel.h"

#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Data/AWeekItemCraftingRecipe.h"
#include "AWeek/UI/Inventory/AWeekItemSlot.h"
#include "Components/HorizontalBox.h"

void UAWeekCraftingDetailPanel::SetRecipe(int32 RecipeIndex, UAWeekCraftingComponent* InCraftingComponent)
{
	const FAWeekCachedCraftingRecipe& ItemRecipe = CraftingComponent->GetRecipeAt(RecipeIndex);
	ResultItemSlot->InitializeItemSlot(ItemRecipe.CraftedItemEntry.ItemData, ItemRecipe.CraftedItemEntry.Quantity);

	IngredientsContainer->ClearChildren();
	for (const FAWeekItemEntry& IngredientItemEntry : ItemRecipe.IngredientItemEntries)
	{
		UAWeekItemSlot* IngredientItemSlot = CreateWidget<UAWeekItemSlot>(this, ItemSlotClass);
		IngredientItemSlot->InitializeItemSlot(IngredientItemEntry.ItemData, IngredientItemEntry.Quantity);
		IngredientsContainer->AddChild(IngredientItemSlot);
	}
}

void UAWeekCraftingDetailPanel::UpdateIngredientSlots()
{
	
}

void UAWeekCraftingDetailPanel::UpdateCraftButton()
{
}

void UAWeekCraftingDetailPanel::OnCraftButtonClicked()
{
}
