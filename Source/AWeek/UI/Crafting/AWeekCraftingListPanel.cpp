// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Crafting/AWeekCraftingListPanel.h"

#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/UI/Crafting/AWeekCraftingItemSlot.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Data/AWeekItemCraftingRecipe.h"
#include "Components/UniformGridPanel.h"

UAWeekCraftingListPanel::UAWeekCraftingListPanel() : NumCols(7)
{
}

void UAWeekCraftingListPanel::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshCraftingList();
}

void UAWeekCraftingListPanel::RefreshCraftingList()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	AAWeekPlayerCharacter* PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}
	CraftingComponent = PlayerCharacter->GetCraftingComponent();
	if (IsValid(CraftingComponent) && GridPanel)
	{
		GridPanel->ClearChildren();
		
		TArray<int32> AvailableRecipes = CraftingComponent->GetAvailableRecipes();
		for (int i = 0; i < AvailableRecipes.Num(); i++)
		{
			UAWeekCraftingItemSlot* ItemRecipeSlotWidget = CreateWidget<UAWeekCraftingItemSlot>(this, CraftingSlotClass);

			int32 RecipeIndex = AvailableRecipes[i];
			FAWeekCachedCraftingRecipe Recipe;
			if (!CraftingComponent->GetRecipeAt(RecipeIndex, Recipe))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s: Fail to get recipe=%d"), *FString(__FUNCTION__), RecipeIndex);
				continue;
			}
			bool bIsCraftable = CraftingComponent->CanCraft(RecipeIndex);
			ItemRecipeSlotWidget->InitializeCraftingItemSlot(RecipeIndex,
				Recipe.CraftedItemEntry.ItemData,
				Recipe.CraftedItemEntry.Quantity,
				bIsCraftable);

			ItemRecipeSlotWidget->OnCraftingSlotLeftClicked.AddLambda([this](int32 RecipeIndex, bool bIsCraftable)
			{
				OnRecipeSelected.Broadcast(RecipeIndex, bIsCraftable);
			});
			GridPanel->AddChildToUniformGrid(ItemRecipeSlotWidget, i / NumCols, i % NumCols);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Crafting Component is null!"), *FString(__FUNCTION__));
	}
}