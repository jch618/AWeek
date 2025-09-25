// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Crafting/AWeekCraftingMainPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingDetailPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingListPanel.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/UI/AWeekGameUIManager.h"

void UAWeekCraftingMainPanel::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("%s: call"), *FString(__FUNCTION__));
	AAWeekPlayerCharacter* PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn());
	CraftingComponent = Cast<UAWeekCraftingComponent>(PlayerCharacter->GetComponentByClass(UAWeekCraftingComponent::StaticClass()));
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
}

void UAWeekCraftingMainPanel::OnRecipeSelected(int32 RecipeIndex)
{
	if (CraftingDetailPanel && CraftingComponent)
	{
		CraftingDetailPanel->SetRecipe(RecipeIndex, CraftingComponent);
	}
}

void UAWeekCraftingMainPanel::ShowDetailPanel()
{
	CraftingDetailPanel->SetVisibility(ESlateVisibility::Visible);
}

