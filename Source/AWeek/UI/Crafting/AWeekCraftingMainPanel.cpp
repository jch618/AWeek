// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Crafting/AWeekCraftingMainPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingDetailPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingListPanel.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/UI/Inventory/AWeekInventoryPanel.h"

void UAWeekCraftingMainPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAWeekCraftingMainPanel::InitializeCraftingMainPanel(const TObjectPtr<UAWeekCraftingComponent> NewCraftingComponent,
	const TObjectPtr<UAWeekInventoryComponent> NewInventoryComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	CraftingComponent = NewCraftingComponent;
	ensure(CraftingComponent);
	if (!CraftingComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Crafting Component is null"), *FString(__FUNCTION__));
	}
	
	if (CraftingListPanel)
	{
		CraftingListPanel->OnRecipeSelected.AddUObject(this, &UAWeekCraftingMainPanel::OnRecipeSelected);
	}

	CraftingListPanel->SetCraftingComponent(CraftingComponent);
	CraftingListPanel->RefreshCraftingList();

	CraftingDetailPanel->OnCraftButtonLeftClicked.AddUObject(this, &UAWeekCraftingMainPanel::OnCraftButtonLeftClicked);

	PlayerInventoryPanel->LinkToInventory(NewInventoryComponent, Cast<AAWeekPlayerCharacter>(NewInventoryComponent->GetOwner()));
}

void UAWeekCraftingMainPanel::OnRecipeSelected(int32 RecipeIndex, bool bIsCraftable)
{
	if (CraftingDetailPanel && CraftingComponent)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s: CraftingComponent RecipesNum:%d"), *FString(__FUNCTION__),
		// 	CraftingComponent->GetCachedCraftingRecipes().Num());
		CraftingDetailPanel->SetRecipe(RecipeIndex, CraftingComponent, bIsCraftable);
	}
}

void UAWeekCraftingMainPanel::OnCraftButtonLeftClicked(int32 RecipeIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Recipe Index: %d"), *FString(__FUNCTION__), RecipeIndex);
    
	if (CraftingComponent)
	{        
		if (CraftingComponent->TryCraftRecipe(RecipeIndex))
		{
			UE_LOG(LogTemp, Warning, TEXT("Crafting Success!"));
			CraftingListPanel->RefreshCraftingList();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Crafting Failed - Insufficient Materials"));
		}
	}
}

void UAWeekCraftingMainPanel::ShowDetailPanel()
{
	CraftingDetailPanel->SetVisibility(ESlateVisibility::Visible);
}


