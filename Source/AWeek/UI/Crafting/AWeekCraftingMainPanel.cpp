// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Crafting/AWeekCraftingMainPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingDetailPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingListPanel.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/UI/Inventory/AWeekInventoryPanel.h"
#include "AWeek/UI/Controller/AWeekCraftingController.h"

void UAWeekCraftingMainPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UAWeekCraftingMainPanel::InitializeCraftingMainPanel(const TObjectPtr<UAWeekCraftingController> InCraftingController,
                                                          const TObjectPtr<UAWeekInventoryComponent> InInventoryComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	CraftingController = InCraftingController;
	if (!CraftingController)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Crafting Controller is null"), *FString(__FUNCTION__));
	}
	
	if (CraftingListPanel)
	{
		CraftingListPanel->OnRecipeSelected.AddUObject(this, &UAWeekCraftingMainPanel::OnRecipeSelected);
	}

	CraftingListPanel->SetCraftingComponent(CraftingController->GetCraftingComponent());
	CraftingListPanel->RefreshCraftingList();

	CraftingDetailPanel->OnCraftButtonLeftClicked.AddUObject(this, &UAWeekCraftingMainPanel::OnCraftButtonLeftClicked);

	PlayerInventoryPanel->LinkToInventory(InInventoryComponent);
}

void UAWeekCraftingMainPanel::OnRecipeSelected(int32 RecipeIndex, bool bIsCraftable)
{
	if (CraftingDetailPanel && CraftingController)
	{
		bIsCraftable = CraftingController->CanCraftRecipe(RecipeIndex);
		CraftingDetailPanel->SetRecipe(RecipeIndex, bIsCraftable);
	}
}

void UAWeekCraftingMainPanel::OnCraftButtonLeftClicked(int32 RecipeIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Recipe Index: %d"), *FString(__FUNCTION__), RecipeIndex);
    
	if (CraftingController)
	{
		FCraftingResult CraftingResult = CraftingController->TryCraftRecipe(RecipeIndex);
		if (CraftingResult.Result == ECraftingFailureReason::Success)
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


