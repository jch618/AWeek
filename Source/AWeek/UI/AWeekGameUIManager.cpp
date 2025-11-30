// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"
#include "AWeek/UI/Inventory/AWeekInventoryHubWidget.h"
#include "AWeek/UI/Inventory/AWeekHeldItem.h"
#include "AWeek/UI/MainWidget/MainUIWidget.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "CommonUIExtensions.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeek/Data/AWeekUIDataAsset.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Grid/GridPlacedSubsystem.h"
#include "AWeek/UI/Building/PreviewObjectWidget.h"
#include "AWeek/UI/Controller/AWeekCraftingController.h"
#include "AWeek/UI/Controller/AWeekInventoryController.h"
#include "AWeek/UI/Building/BuildingSelectWidget.h"
#include "Inventory/AWeekPlayerHotBar.h"

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
			HeldItemVisualClass = UIDataAsset->HeldItemVisualClass;
			MainUIWidgetClass = UIDataAsset->MainWidgetClass;
			BuildingSelectWidgetClass = UIDataAsset->BuildingSelectWidgetClass;
            
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
	InventoryHubWidget = Cast<UAWeekInventoryHubWidget>(
	UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
	FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryHubWidgetClass));
	
	switch (DisplayPanel)
	{
	case EAWeekInventoryHubPanel::Weapon:
		break;
	case EAWeekInventoryHubPanel::Chest:
		InventoryHubWidget->SwitchToPanel(
			EAWeekInventoryHubPanel::Chest,
			FAWeekPanelContext::ForChest(PlayerCharacter->GetChestInventoryComponent())
			);
		break;
	case EAWeekInventoryHubPanel::Crafting:
		// CraftingController->GetCraftingComponent()->UpdateInventoryCounts();
		InventoryHubWidget->SwitchToPanel(
			EAWeekInventoryHubPanel::Crafting,
			FAWeekPanelContext::ForCrafting(CraftingController->GetCraftingComponent())
			);
		break;
	case EAWeekInventoryHubPanel::PlayerState:
		break;
	default:
		// CraftingController->GetCraftingComponent()->UpdateInventoryCounts();
		InventoryHubWidget->SwitchToPanel(
			EAWeekInventoryHubPanel::Crafting,
			FAWeekPanelContext::ForCrafting(CraftingController->GetCraftingComponent())
			);
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
		PlayerCharacter->GetCraftingComponent()->SetCraftingLevel(0);
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

void UAWeekGameUIManager::ShowBuildingWidget()
{
	UE_LOG(LogTemp, Log, TEXT("Show Building UI Widget"));
	if (BuildingSelectWidgetClass)
	{
		
		BuildingSelectWidget = Cast<UBuildingSelectWidget, UCommonActivatableWidget>(
		UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
			FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), BuildingSelectWidgetClass));

		/*UE_LOG(LogTemp, Log, TEXT("MainUIWidgetClass=%s IsChildOf Activatable? %s"),
	*GetNameSafe(*MainUIWidgetClass),
	(MainUIWidgetClass && MainUIWidgetClass->IsChildOf(UCommonActivatableWidget::StaticClass())) ? TEXT("YES") : TEXT("NO"));*/
	}
}

void UAWeekGameUIManager::HideBuildingWidget()
{
	if (BuildingSelectWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("BuildingWidget HIDE"));
		BuildingSelectWidget->DeactivateWidget();
		InventoryController->ReturnHeldItemToInventory();
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

void UAWeekGameUIManager::ToggleBuildingWidget()
{
	UE_LOG(LogTemp, Log, TEXT("Toggle Building Widget Test!!"));
	if (!IsValid(BuildingSelectWidget)|| !BuildingSelectWidget->IsActivated())
	{
		ShowBuildingWidget();
		PlayerController->SetShowMouseCursor(false);
		if (UWorld* World = GetWorld())
		{
			if (UGridPlacedSubsystem* Grid = World->GetSubsystem<UGridPlacedSubsystem>())
			{
				Grid->StopPlacement();
			}
		}
	}else
	{
		HideBuildingWidget();
		//PlayerController->SetShowMouseCursor(false);
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