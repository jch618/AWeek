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
}

void UAWeekCraftingDetailPanel::InitializeCraftingDetailPanel()
{
	if (CraftButton)
	{
		CraftButton->OnClicked.AddDynamic(this, &UAWeekCraftingDetailPanel::OnCraftButtonClicked);
	}
	
	IngredientsContainer->ClearChildren();	
}

void UAWeekCraftingDetailPanel::SetRecipe(int32 RecipeIndex, bool bIsCraftable)
{
	FAWeekCachedCraftingRecipe ItemRecipe;
	if (CraftingComponent && CraftingComponent->GetRecipeAt(RecipeIndex, ItemRecipe))
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

void UAWeekCraftingDetailPanel::UpdateCraftButton()
{
	if (CraftingComponent && CurrentRecipeIndex != -1)
	{
		CraftButton->SetIsEnabled(CraftingComponent->CanCraft(CurrentRecipeIndex));
	}
}

void UAWeekCraftingDetailPanel::ClearCraftingDetails()
{
	
}

void UAWeekCraftingDetailPanel::UpdateIngredientSlots()
{
	
}

void UAWeekCraftingDetailPanel::OnCraftButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	OnCraftButtonLeftClicked.Broadcast(CurrentRecipeIndex);
}