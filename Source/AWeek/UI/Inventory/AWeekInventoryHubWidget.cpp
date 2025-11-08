// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekInventoryHubWidget.h"
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

void UAWeekInventoryHubWidget::NativeDestruct()
{
	UE_LOG(LogTemp, Warning, TEXT("%s:"), *FString(__FUNCTION__));
	// if (CraftingDetailPanel)
	// {
	// 	CraftingDetailPanel->OnCraftButtonLeftClicked.RemoveAll(this);
	// }
 //    
	// if (CraftingController && CraftingController->GetCraftingComponent())
	// {
	// 	CraftingController->GetCraftingComponent()->OnCraftingFinished.RemoveAll(this);
	// 	CraftingController->GetCraftingComponent()->OnCraftingFinished.RemoveAll(CraftingListPanel);
	// 	CraftingController->GetCraftingComponent()->OnCraftingFinished.RemoveAll(CraftingDetailPanel);
	// }
	Super::NativeDestruct();

}

void UAWeekInventoryHubWidget::InitializeInventoryHub(TObjectPtr<UAWeekCraftingController> InCraftingController, const TObjectPtr<UAWeekInventoryComponent> InInventoryComponent)
{
	if (InCraftingController)
	{
		CraftingController = InCraftingController;
		InitializeCraftingPanel();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: CraftingController is null!"), *FString(__FUNCTION__));
	}

	PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn());
	if (InInventoryComponent)
	{
		InitializeInventoryPanel();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: InventoryComponent is null!"), *FString(__FUNCTION__));
	}
	
	// Initialize buttons
	if (ChestPanelButton)
	{
		ChestPanelButton->OnClicked().AddUObject(this, &UAWeekInventoryHubWidget::SwitchToChestPanel);
		// ChestPanelButton->OnClicked().AddLambda([this]()
		// {
		// 	SwitchToPanel(EAWeekInventoryHubPanel::Chest, FAWeekPanelContext());
		// });
	}
	if (CraftingListPanel)
	{
		CraftingPanelButton->OnClicked().AddUObject(this, &UAWeekInventoryHubWidget::SwitchToCraftingListPanel);
		// CraftingPanelButton->OnClicked().AddLambda([this]()
		// {
		// 	SwitchToPanel(EAWeekInventoryHubPanel::Crafting, FAWeekPanelContext());
		// });
	}	
}

void UAWeekInventoryHubWidget::OpenChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventoryComponent)
{
	if (ChestInventoryPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
		ChestInventoryPanel->LinkToInventory(ChestInventoryComponent);
		SwitchToChestPanel();
		bIsChestOpen = true;
	}
	RefreshInventoryHub();
}

void UAWeekInventoryHubWidget::CloseChestInventory()
{
	if (ChestInventoryPanel)
	{
		ChestInventoryPanel->UnlinkFromInventory();
		bIsChestOpen = false;
		ChestPanelButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	RefreshInventoryHub();
	
}

void UAWeekInventoryHubWidget::OpenCraftingPanel()
{
	if (CraftingListPanel)
	{
		SwitchToCraftingListPanel();
		CraftingListPanel->RefreshCraftingList();
	}
	RefreshInventoryHub();
	
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
	UE_LOG(LogTemp, Warning, TEXT("%s: Recipe Index: %d"), *FString(__FUNCTION__), RecipeIndex);
    
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

void UAWeekInventoryHubWidget::SwitchToChestPanel() const
{
	HideCraftingDetailPanel();
	if (ChestInventoryPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
		PanelSwitcher->SetActiveWidget(ChestInventoryPanel);
	}
}

void UAWeekInventoryHubWidget::SwitchToCraftingListPanel() const
{
	if (CraftingListPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
		PanelSwitcher->SetActiveWidget(CraftingListPanel);
	}
}

void UAWeekInventoryHubWidget::SwitchToPanel(EAWeekInventoryHubPanel TargetPanel, const FAWeekPanelContext& PanelContext)
{
	HideCraftingDetailPanel();

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
	// switch (TargetPanel)
	// {
	// case EAWeekInventoryHubPanel::Weapon:
	// 	if (CraftingListPanel)
	// 	{
	// 		SwitchToCraftingListPanel();
	// 		CraftingListPanel->RefreshCraftingList();
	// 	}
	// 	RefreshInventoryHub();
	// 	break;
	// case EAWeekInventoryHubPanel::Chest:
	// 	if (ChestInventoryPanel)
	// 	{
	// 		ChestInventoryPanel->LinkToInventory(PanelContext.ChestInventoryComponent);
	// 		PanelSwitcher->SetActiveWidget(ChestInventoryPanel);
	// 		bIsChestOpen = true;
	// 	}
	// 	RefreshInventoryHub();
	// 	break;
	// case EAWeekInventoryHubPanel::Crafting:
	// 	ShowCraftingDetailPanel();
	// 	PanelSwitcher->SetActiveWidget(ChestInventoryPanel);
	// 	break;
	// case EAWeekInventoryHubPanel::PlayerState:
	// 	break;
	// default:
	// 	break;
	// 	
	// }
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
	if (bIsChestOpen)
	{
		ChestPanelButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ChestPanelButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAWeekInventoryHubWidget::ActivatePanel(EAWeekInventoryHubPanel Panel, const FAWeekPanelContext& Context) const
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
		ShowCraftingDetailPanel();
		PanelSwitcher->SetActiveWidget(ChestInventoryPanel);
		break;
	case EAWeekInventoryHubPanel::PlayerState:
		break;
	}
}

void UAWeekInventoryHubWidget::DeactivatePanel(EAWeekInventoryHubPanel Panel) const
{
	switch (Panel)
	{
	case EAWeekInventoryHubPanel::None:
		break;
	case EAWeekInventoryHubPanel::Weapon:
		break;
	case EAWeekInventoryHubPanel::Chest:
		if (ChestInventoryPanel)
		{
			ChestInventoryPanel->UnlinkFromInventory();
		}
		break;
	case EAWeekInventoryHubPanel::Crafting:
		HideCraftingDetailPanel();
		break;
	case EAWeekInventoryHubPanel::PlayerState:
		break;
	}
}

void UAWeekInventoryHubWidget::UpdatePanelButtons()
{
	if (ChestPanelButton)
	{
		ChestPanelButton->SetVisibility(bIsChestOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	
	if (CraftingPanelButton)
	{
		CraftingPanelButton->SetIsSelected(CurrentPanel == EAWeekInventoryHubPanel::Crafting);
	}	
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
	PlayerInventoryPanel->LinkToInventory(PlayerCharacter->GetPlayerInventoryComponent(), PlayerCharacter);
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
		CraftingController->GetCraftingComponent()->OnCraftingFinished.AddUObject(
			CraftingListPanel, &UAWeekCraftingListPanel::RefreshCraftingList);
		CraftingController->GetCraftingComponent()->OnCraftingFinished.AddUObject(
			CraftingDetailPanel, &UAWeekCraftingDetailPanel::UpdateCraftButton);
	}
}
