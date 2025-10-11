// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Crafting/AWeekCraftingDetailPanel.h"

#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Data/AWeekItemCraftingRecipe.h"
#include "AWeek/UI/Inventory/AWeekItemSlot.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"

void UAWeekCraftingDetailPanel::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (CraftButton)
	{
		CraftButton->OnClicked.AddDynamic(this, &UAWeekCraftingDetailPanel::OnCraftButtonClicked);
	}
}

void UAWeekCraftingDetailPanel::SetRecipe(int32 RecipeIndex, const UAWeekCraftingComponent* CraftingComponent, bool bIsCraftable)
{
	FAWeekCachedCraftingRecipe ItemRecipe;
	if (CraftingComponent->GetRecipeAt(RecipeIndex, ItemRecipe))
	{
		CurrentRecipeIndex = RecipeIndex;
		
		ResultItemSlot->InitializeItemSlot(ItemRecipe.CraftedItemEntry.ItemData, ItemRecipe.CraftedItemEntry.Quantity);

		IngredientsContainer->ClearChildren();
		for (const FAWeekItemEntry& IngredientItemEntry : ItemRecipe.IngredientItemEntries)
		{
			UAWeekItemSlot* IngredientItemSlot = CreateWidget<UAWeekItemSlot>(this, ItemSlotClass);
			IngredientItemSlot->InitializeItemSlot(IngredientItemEntry.ItemData, IngredientItemEntry.Quantity);
			IngredientsContainer->AddChild(IngredientItemSlot);
		}

		CraftButton->SetIsEnabled(bIsCraftable);
	}
	else
	{
		CraftButton->SetIsEnabled(false);
	}
}

void UAWeekCraftingDetailPanel::ClearCraftingDetails()
{
	
}


void UAWeekCraftingDetailPanel::UpdateIngredientSlots()
{
	
}

void UAWeekCraftingDetailPanel::UpdateCraftButton()
{
}

void UAWeekCraftingDetailPanel::OnCraftButtonClicked()
{
	OnCraftButtonLeftClicked.Broadcast(CurrentRecipeIndex);
}
