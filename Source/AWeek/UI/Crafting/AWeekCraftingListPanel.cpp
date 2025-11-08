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
	CraftingComponent = PlayerCharacter->GetCraftingComponent();
	if (IsValid(CraftingComponent) && GridPanel)
	{
		GridPanel->ClearChildren();

		const TArray<FAWeekCachedCraftingRecipe>& CachedCraftingRecipes = CraftingComponent->GetCachedCraftingRecipes();
		for (int i = 0; i < CachedCraftingRecipes.Num(); i++)
		{		
			UAWeekCraftingItemSlot* ItemRecipeSlotWidget = CreateWidget<UAWeekCraftingItemSlot>(this, CraftingSlotClass);

			bool bIsCraftable = CraftingComponent->CanCraft(i);
			ItemRecipeSlotWidget->InitializeCraftingItemSlot(i,
				CachedCraftingRecipes[i].CraftedItemEntry.ItemData,
				CachedCraftingRecipes[i].CraftedItemEntry.Quantity,
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