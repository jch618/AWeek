// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekInventoryHubWidget.h"

#include "AWeekPlayerInventoryPanel.h"
#include "AWeek/UI/Inventory/AWeekInventoryPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingDetailPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingListPanel.h"
#include "AWeek/UI/Controller/AWeekCraftingController.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "CommonButtonBase.h"

#include "CommonUIExtensions.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/UI/MainWidget/MainUIWidget.h"

#include "Components/CanvasPanel.h"


void UAWeekInventoryHubWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HideCraftingDetailPanel();
}

void UAWeekInventoryHubWidget::InitializeInventoryHub(TObjectPtr<UAWeekCraftingController> InCraftingController,
	const TObjectPtr<UAWeekPlayerInventoryComponent> InPlayerInventoryComponent)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	
	if (InCraftingController)
	{
		CraftingController = InCraftingController;
		InitializeCraftingPanel();
	}

	PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn());
	if (InPlayerInventoryComponent)
	{
		InitializeInventoryPanel();
	}
	
	// Initialize buttons
	if (ChestPanelButton)
	{
		ChestPanelButton->OnClicked().AddLambda([this]()
		{
			SwitchToPanel(EAWeekInventoryHubPanel::Chest, FAWeekPanelContext());
		});
	}
	if (CraftingListPanel)
	{
		CraftingPanelButton->OnClicked().AddLambda([this]()
		{
			SwitchToPanel(EAWeekInventoryHubPanel::Crafting, FAWeekPanelContext());
		});
	}	
}

void UAWeekInventoryHubWidget::OnRecipeSelected(int32 RecipeIndex, bool bIsCraftable)
{
	ShowCraftingDetailPanel();
	if (CraftingDetailPanel && CraftingController)
	{
		bIsCraftable = CraftingController->CanCraftRecipe(RecipeIndex);
		CraftingDetailPanel->SetCraftingComponent(CraftingController->GetCraftingComponent());
		CraftingDetailPanel->SetRecipe(RecipeIndex, bIsCraftable);
	}
}

void UAWeekInventoryHubWidget::OnCraftButtonLeftClicked(int32 RecipeIndex) const
{
	// UE_LOG(LogTemp, Warning, TEXT("%s: Recipe Index: %d"), *FString(__FUNCTION__), RecipeIndex);
    
	if (CraftingController)
	{
		FCraftingResult CraftingResult = CraftingController->TryCraftRecipe(RecipeIndex);
		if (CraftingResult.Result == ECraftingFailureReason::Success)
		{
			UE_LOG(LogTemp, Warning, TEXT("Crafting Success!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Crafting Failed"));
		}
	}
}


void UAWeekInventoryHubWidget::SwitchToPanel(EAWeekInventoryHubPanel TargetPanel, const FAWeekPanelContext& PanelContext)
{
	if (CurrentPanel == TargetPanel)
	{
		return;
	}

	if (CurrentPanel != EAWeekInventoryHubPanel::None)
	{
		DeactivatePanel(CurrentPanel);
	}

	PreviousPanel = CurrentPanel;
	CurrentPanel = TargetPanel;

	if (TargetPanel != EAWeekInventoryHubPanel::None)
	{
		ActivatePanel(TargetPanel, PanelContext);
	}
}

void UAWeekInventoryHubWidget::CloseCurrentPanel()
{
	if (CurrentPanel != EAWeekInventoryHubPanel::None)
	{
		DeactivatePanel(CurrentPanel);
		CurrentPanel = EAWeekInventoryHubPanel::None;
	}
}

void UAWeekInventoryHubWidget::RefreshInventoryHub()
{
	if (ChestInventoryPanel->IsLinkedToInventory())
	{
		ChestPanelButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ChestPanelButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAWeekInventoryHubWidget::ActivatePanel(EAWeekInventoryHubPanel Panel, const FAWeekPanelContext& Context)
{
	switch (Panel)
	{
	case EAWeekInventoryHubPanel::None:
		break;
	case EAWeekInventoryHubPanel::Weapon:
		break;
	case EAWeekInventoryHubPanel::Chest:
		if (Context.ChestInventoryComponent)
		{
			ChestInventoryPanel->LinkToInventory(Context.ChestInventoryComponent);
		}
		PanelSwitcher->SetActiveWidget(ChestInventoryPanel);
		break;
	case EAWeekInventoryHubPanel::Crafting:
		CraftingController->GetCraftingComponent()->UpdateInventoryCounts();
		CraftingListPanel->RefreshCraftingList();
		PanelSwitcher->SetActiveWidget(CraftingListPanel);
		break;
	case EAWeekInventoryHubPanel::PlayerState:
		break;
	}
	UpdatePanelButtons();
}

void UAWeekInventoryHubWidget::DeactivatePanel(EAWeekInventoryHubPanel Panel)
{
	switch (Panel)
	{
	case EAWeekInventoryHubPanel::None:
		break;
	case EAWeekInventoryHubPanel::Weapon:
		break;
	case EAWeekInventoryHubPanel::Chest:
		break;
	case EAWeekInventoryHubPanel::Crafting:
		HideCraftingDetailPanel();
		break;
	case EAWeekInventoryHubPanel::PlayerState:
		break;
	}
	UpdatePanelButtons();
}

void UAWeekInventoryHubWidget::UpdatePanelButtons()
{
	if (ChestPanelButton)
	{
		ChestPanelButton->SetVisibility(IsChestOpen() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		ChestPanelButton->SetIsSelected(CurrentPanel == EAWeekInventoryHubPanel::Chest);
	}
	
	if (CraftingPanelButton)
	{
		CraftingPanelButton->SetIsSelected(CurrentPanel == EAWeekInventoryHubPanel::Crafting);
	}	
}
void UAWeekInventoryHubWidget::CloseChestInventory()
{
	if (!IsChestOpen())
	{
		return;
	}
	if (CurrentPanel == EAWeekInventoryHubPanel::Chest)
	{
		SwitchToPanel(EAWeekInventoryHubPanel::Crafting, FAWeekPanelContext());
	}
	ChestInventoryPanel->UnlinkFromInventory();
	UpdatePanelButtons();
}

void UAWeekInventoryHubWidget::ShowCraftingDetailPanel() const
{
	CraftingDetailPanel->SetVisibility(ESlateVisibility::Visible);
}

void UAWeekInventoryHubWidget::HideCraftingDetailPanel() const
{
	CraftingDetailPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UAWeekInventoryHubWidget::InitializeInventoryPanel() const
{
	PlayerInventoryPanel->LinkToInventory(PlayerCharacter->GetPlayerInventoryComponent());
}

void UAWeekInventoryHubWidget::InitializeCraftingPanel()
{
	if (CraftingListPanel && CraftingDetailPanel && CraftingController)
	{
		CraftingListPanel->OnRecipeSelected.AddUObject(this, &UAWeekInventoryHubWidget::OnRecipeSelected);
		CraftingListPanel->SetCraftingComponent(CraftingController->GetCraftingComponent());
		CraftingListPanel->RefreshCraftingList();

		CraftingDetailPanel->InitializeCraftingDetailPanel();
		CraftingDetailPanel->SetCraftingComponent(CraftingController->GetCraftingComponent());
		CraftingDetailPanel->OnCraftButtonLeftClicked.AddUObject(this, &UAWeekInventoryHubWidget::OnCraftButtonLeftClicked);

		// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
		CraftingController->GetCraftingComponent()->OnCraftingFinished.AddUObject(
			CraftingListPanel, &UAWeekCraftingListPanel::RefreshCraftingList);
		CraftingController->GetCraftingComponent()->OnCraftingFinished.AddUObject(
			CraftingDetailPanel, &UAWeekCraftingDetailPanel::UpdateCraftButton);
		CraftingController->GetCraftingComponent()->OnCraftingLevelChanged.AddUObject(
			CraftingListPanel, &UAWeekCraftingListPanel::RefreshCraftingList);
	}
}
