// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/UI/Inventory/AWeekInventoryMainPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingMainPanel.h"
#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"
#include "AWeek/UI/Inventory/AWeekInventoryHubWidget.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"
#include "AWeek/UI/Inventory/AWeekHeldItem.h"
#include "AWeek/UI/MainWidget/MainUIWidget.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "CommonUIExtensions.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeek/Data/AWeekUIDataAsset.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Grid/GridPlacedSubsystem.h"
#include "AWeek/UI/Building/PreviewObjectWidget.h"
#include "AWeek/UI/Controller/AWeekCraftingController.h"
#include "AWeek/UI/Controller/AWeekInventoryController.h"

UAWeekGameUIManager::UAWeekGameUIManager()
{
	UIDataAssetPath = FSoftObjectPath(TEXT("/Game/Data/UI/UA_UIClassSettings.UA_UIClassSettings"));
}

void UAWeekGameUIManager::InitializeUIManager(const TObjectPtr<AAWeekPlayerCharacter> InPlayerCharacter)
{
	PlayerController = Cast<AAWeekPlayerController>(GetWorld()->GetFirstPlayerController());
	LocalPlayer = PlayerController->GetLocalPlayer();
	PlayerCharacter = InPlayerCharacter;
	
	if (UIDataAssetPath.IsValid())
	{
		UIDataAsset = Cast<UAWeekUIDataAsset>(UIDataAssetPath.TryLoad());
        
		if (UIDataAsset)
		{
			InventoryHubWidgetClass = UIDataAsset->InventoryHubWidgetClass;
			InventoryMainPanelClass = UIDataAsset->InventoryMainPanelClass;
			CraftingMainPanelClass = UIDataAsset->CraftingMainPanelClass;
			InteractionWidgetClass = UIDataAsset->InteractionWidgetClass;
			HeldItemVisualClass = UIDataAsset->HeldItemVisualClass;
			MainUIWidgetClass = UIDataAsset->MainWidgetClass;
            
			UE_LOG(LogTemp, Log, TEXT("UI DataAsset loaded from: %s"), *UIDataAssetPath.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load UI DataAsset from: %s"), *UIDataAssetPath.ToString());
			return;
		}
	}

	// Create UI Controllers
	InventoryController = NewObject<UAWeekInventoryController>(this, UAWeekInventoryController::StaticClass());
	InventoryController->InitializeInventoryController(this, HeldItemVisualClass);

	CraftingController = NewObject<UAWeekCraftingController>(this, UAWeekCraftingController::StaticClass());
	CraftingController->InitializeCraftingController(this,
		PlayerCharacter->GetCraftingComponent(),
		PlayerCharacter->GetPlayerInventoryComponent());
	
	if (InventoryHubWidgetClass)
	{
		InventoryHubWidget = Cast<UAWeekInventoryHubWidget, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryHubWidgetClass));
		InventoryHubWidget->InitializeInventoryHub(CraftingController, InPlayerCharacter->GetPlayerInventoryComponent());
		InventoryHubWidget->DeactivateWidget();
	}
}

void UAWeekGameUIManager::ToggleInventoryHub(EAWeekInventoryHubPanel DisplayPanel)
{
	if (!IsInventoryHubOpen())
	{
		OpenInventoryHub(DisplayPanel);
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		CloseInventoryHub();
		PlayerController->SetShowMouseCursor(false);
	}
}

void UAWeekGameUIManager::OpenInventoryHub(EAWeekInventoryHubPanel DisplayPanel)
{	
	if (!InventoryHubWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: InventoryHubWidgetClass is null!"), *FString(__FUNCTION__));
		return;
	}
	InventoryHubWidget = Cast<UAWeekInventoryHubWidget, UCommonActivatableWidget>(
		UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
			FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryHubWidgetClass));
	
	switch (DisplayPanel)
	{
	case EAWeekInventoryHubPanel::Weapon:
		break;
	case EAWeekInventoryHubPanel::Chest:
		// InventoryHubWidget->SwitchToPanel(
		// 	EAWeekInventoryHubPanel::Chest,
		// 	FAWeekPanelContext::ForChest(PlayerCharacter->GetChestInventoryComponent())
		// 	);
		OpenChestInventory(PlayerCharacter->GetChestInventoryComponent());
		break;
	case EAWeekInventoryHubPanel::Crafting:
		CraftingController->GetCraftingComponent()->UpdateInventoryCounts();
		InventoryHubWidget->OpenCraftingPanel();
		// InventoryHubWidget->SwitchToPanel(
		// 	EAWeekInventoryHubPanel::Chest,
		// 	FAWeekPanelContext::ForCrafting(CraftingController->GetCraftingComponent())
		// 	);
		break;
	case EAWeekInventoryHubPanel::PlayerState:
		break;
	default:
		break;
	}
}

void UAWeekGameUIManager::CloseInventoryHub()
{
	if (InventoryHubWidget)
	{
		if (InventoryHubWidget->IsChestOpen())
		{
			CloseChestInventory();
		}
		InventoryHubWidget->DeactivateWidget();
		InventoryController->ReturnHeldItemToInventory();
	}
}

void UAWeekGameUIManager::ShowMainWidget()
{
	UE_LOG(LogTemp, Log, TEXT("1asdf"));
	if (MainUIWidgetClass)
	{
		UE_LOG(LogTemp, Log, TEXT("2asdf"));
		MainUIWidget = Cast<UMainUIWidget, UCommonActivatableWidget>(
		UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
			FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), MainUIWidgetClass));

		UE_LOG(LogTemp, Log, TEXT("MainUIWidgetClass=%s IsChildOf Activatable? %s"),
	*GetNameSafe(*MainUIWidgetClass),
	(MainUIWidgetClass && MainUIWidgetClass->IsChildOf(UCommonActivatableWidget::StaticClass())) ? TEXT("YES") : TEXT("NO"));
	}
}

void UAWeekGameUIManager::HideMainWidget()
{
	if (MainUIWidget)
	{
		MainUIWidget->DeactivateWidget();
		InventoryController->ReturnHeldItemToInventory();
	}
}

bool UAWeekGameUIManager::IsInventoryHubOpen() const
{
	return IsValid(InventoryHubWidget) && InventoryHubWidget->IsActivated();
}

void UAWeekGameUIManager::ToggleMainWidget()
{
	if (!IsValid(MainUIWidget) || !MainUIWidget->IsActivated())
	{
		ShowMainWidget();
		PlayerController->SetShowMouseCursor(true);
		if (UWorld* World = GetWorld())
		{
			if (UGridPlacedSubsystem* Grid = World->GetSubsystem<UGridPlacedSubsystem>())
			{
				Grid->StopPlacement();
			}
		}
	}else
	{
		HideMainWidget();
		PlayerController->SetShowMouseCursor(false);
	}
}

void UAWeekGameUIManager::PreviewObjectRotateL()
{
	if (!IsValid(PreviewObjectWidget) || !PreviewObjectWidget->IsActivated())
	{
		
	}
}

void UAWeekGameUIManager::PreviewObjectRotateR()
{
	if (!IsValid(PreviewObjectWidget) || !PreviewObjectWidget->IsActivated())
	{
		
	}
}

void UAWeekGameUIManager::ShowInteractionWidget()
{
	if (InteractionWidgetClass)
	{
		InteractionWidget = Cast<UAWeekInteractionWidget, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
				FGameplayTag::RequestGameplayTag("UI.Layer.Game"), InteractionWidgetClass));
	}
}
void UAWeekGameUIManager::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->DeactivateWidget();
	}
}
void UAWeekGameUIManager::UpdateInteractionWidget(const FAWeekInteractableData* InteractableData)
{
	if (InteractionWidget == nullptr || !InteractionWidget->IsActivated())
	{
		if (InteractionWidgetClass)
		{
			ShowInteractionWidget();
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}
//
void UAWeekGameUIManager::OpenChestInventory(const TObjectPtr<UAWeekInventoryComponent> ChestInventory) const
{
	InventoryHubWidget->OpenChestInventory(ChestInventory);
}

void UAWeekGameUIManager::CloseChestInventory()
{
	PlayerCharacter->SetChestInventoryComponent(nullptr);
	if (InventoryController->IsHoldingItem())
	{
		if (InventoryController->GetHeldItem()->GetSourceInventory() == PlayerCharacter->GetChestInventoryComponent())
		{
			InventoryController->ReturnHeldItemToInventory();
		}
	}
	InventoryHubWidget->CloseChestInventory();
}