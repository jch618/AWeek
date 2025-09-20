// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/AWeekGameUIManager.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Player/AWeekUIController.h"
#include "AWeek/UI/AWeekInventoryMainPanel.h"
#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"
#include "AWeek/UI/Inventory/AWeekHeldItem.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "CommonUIExtensions.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeek/Data/AWeekUIDataAsset.h"

UAWeekGameUIManager::UAWeekGameUIManager()
{
	UIDataAssetPath = FSoftObjectPath(TEXT("/Game/Data/UI/UA_UIClassSettings.UA_UIClassSettings"));
}

void UAWeekGameUIManager::InitializeUIManager()
{
	PlayerController = Cast<AAWeekPlayerController>(GetWorld()->GetFirstPlayerController());
	LocalPlayer = PlayerController->GetLocalPlayer();

	if (UIDataAssetPath.IsValid())
	{
		UIDataAsset = Cast<UAWeekUIDataAsset>(UIDataAssetPath.TryLoad());
        
		if (UIDataAsset)
		{
			InventoryMainPanelClass = UIDataAsset->InventoryMainPanelClass;
			InteractionWidgetClass = UIDataAsset->InteractionWidgetClass;
			HeldItemVisualClass = UIDataAsset->HeldItemVisualClass;
            
			UE_LOG(LogTemp, Log, TEXT("UI DataAsset loaded from: %s"), *UIDataAssetPath.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load UI DataAsset from: %s"), *UIDataAssetPath.ToString());
			return;
		}
	}
	
	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
		InventoryMainPanelWidget->InitializeInventoryMainPanel();
		InventoryMainPanelWidget->DeactivateWidget();
	}
}
// =====================================================
// INVENTORY SYSTEM
// =====================================================

void UAWeekGameUIManager::ShowMainPanel()
{
	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
	}
}
void UAWeekGameUIManager::HideMainPanel()
{
	if (InventoryMainPanelWidget)
	{
		InventoryMainPanelWidget->DeactivateWidget();
		if (IsHoldingItem())
		{
			HeldItem->ReturnHeldItemToInventory();
			HeldItem = nullptr;
		}
	}
}

void UAWeekGameUIManager::ToggleMainPanel()
{
	if (!IsValid(InventoryMainPanelWidget) || !InventoryMainPanelWidget->IsActivated())
	{
		ShowMainPanel();
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		if (InventoryMainPanelWidget->IsChestOpen())
		{
			DeactivateChestInventory();
		}
		HideMainPanel();
		PlayerController->SetShowMouseCursor(false);
	}
}

void UAWeekGameUIManager::ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
{
	if (!InventoryMainPanelWidget->IsChestOpen())
	{
		if (!IsValid(InventoryMainPanelWidget) || !InventoryMainPanelWidget->IsActivated())
		{
			ShowMainPanel();
			PlayerController->SetShowMouseCursor(true);
		}
		ActivateChestInventory(ChestInventory);
	}
	else
	{
		DeactivateChestInventory();
		HideMainPanel();
		PlayerController->SetShowMouseCursor(false);
	}
}


// void UAWeekGameUIManager::ShowCrosshair()
// {
// 	if (CrosshairWidget)
// 	{
// 		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
// 	}
// }
//
// void UAWeekGameUIManager::HideCrosshair()
// {
// 	if (CrosshairWidget)
// 	{
// 		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// }

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

void UAWeekGameUIManager::ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory) const
{
	InventoryMainPanelWidget->ActivateChestInventory(ChestInventory);
}

void UAWeekGameUIManager::DeactivateChestInventory()
{
	if (IsHoldingItem())
	{
		if (HeldItem->GetSourceInventory() == InventoryMainPanelWidget->GetChestInventoryComponent())
		{
			HeldItem->ReturnHeldItemToInventory();
			HeldItem = nullptr;
		}
	}
	InventoryMainPanelWidget->DeActivateChestInventory();
}

bool UAWeekGameUIManager::IsHoldingItem() const
{
	return IsValid(HeldItem);
}

void UAWeekGameUIManager::UpdateHeldItemPosition(FVector2D NewPosition)
{
	if (IsValid(HeldItem))
	{
		HeldItem->UpdateHeldVisualPosition(NewPosition);
	}
}

void UAWeekGameUIManager::HandleItemSlotLeftClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	// if isHolding item
	//		if heldItem == slotItem => add item to SlotItem as possible and hold remaining items
	//		else => put HeldItem and hold SlotItem
	// else
	//		hold SlotItem

	if (IsHoldingItem())
	{
		const FAWeekItemSlot& ClickedItemSlot = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
		if (ClickedItemSlot.bIsEmpty)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Place Item to slot and clear HeldItem"), *FString(__FUNCTION__));

			OwningInventory->PlaceItemAt(HeldItem->ReleaseHeldItem(), ClickedItemSlotIndex);
			HeldItem->ClearHeldItem();
			HeldItem = nullptr;
		}
		else
		{
			// if target slot item is same as clicked item, merge item quantity as possible and hold remaining quantity of item 
			if (ClickedItemSlot.Item->ID == HeldItem->GetItemReference()->ID)
			{
				MergeItem(ClickedItemSlotIndex, OwningInventory);
			}
			// put held item to target slot and hold target slot item
			else
			{
				UAWeekItemBase* ClickedItem = OwningInventory->ReleaseItemAt(ClickedItemSlotIndex);
				OwningInventory->PlaceItemAt(HeldItem->ReleaseHeldItem(), ClickedItemSlotIndex);
				HeldItem->SetItemReference(ClickedItem);
			}
		}
	}
	else
	{
		if (!OwningInventory->GetItemSlotAt(ClickedItemSlotIndex).bIsEmpty)
		{
			UAWeekItemBase* NewHeldItem = OwningInventory->ReleaseItemAt(ClickedItemSlotIndex);
			CreateHeldItem(NewHeldItem, OwningInventory, ClickedItemSlotIndex);
		}
	}
}

void UAWeekGameUIManager::HandleItemSlotRightClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	const FAWeekItemSlot& ClickedItemSlot = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
	if (IsHoldingItem())
	{
		if (!ClickedItemSlot.bIsEmpty)
		{
			if (HeldItem->GetItemReference()->ID == ClickedItemSlot.Item->ID)
			{
				OwningInventory->RemoveAmountOfItem(ClickedItemSlotIndex, 1);
				HeldItem->SetHeldItemQuantity(HeldItem->GetItemReference()->Quantity + 1);
			}
		}
	}
	else
	{
		if (!ClickedItemSlot.bIsEmpty)
		{
			// hold one item
			UAWeekItemBase* NewHeldItem = ClickedItemSlot.Item->CreateItemCopy();
			NewHeldItem->SetQuantity(1);
			CreateHeldItem(NewHeldItem, OwningInventory, ClickedItemSlotIndex);
			OwningInventory->RemoveAmountOfItem(ClickedItemSlotIndex, 1);
		}
	}
}

void UAWeekGameUIManager::MergeItem(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Merge Item"), *FString(__FUNCTION__));

	int32 HeldItemQuantity = HeldItem->GetItemReference()->Quantity;
	int32 ActualAmountAdded = OwningInventory->AddItemQuantityAt(ClickedItemSlotIndex, HeldItemQuantity);
	HeldItemQuantity -= ActualAmountAdded;
	HeldItem->SetHeldItemQuantity(HeldItemQuantity);
	if (HeldItem->IsEmpty())
	{
		HeldItem = nullptr;
	}
}

void UAWeekGameUIManager::CreateHeldItem(TObjectPtr<UAWeekItemBase> NewHeldItem, TObjectPtr<UAWeekInventoryComponent> SourceInventory, int32 SourceItemSlotIndex)
{
	if (HeldItemVisualClass)
	{
		UAWeekHeldItemVisual* HeldItemVisual = Cast<UAWeekHeldItemVisual>(CreateWidget(PlayerController, HeldItemVisualClass));
		HeldItem = NewObject<UAWeekHeldItem>();

		if (HeldItemVisual)
		{
			FHeldItemData HeldItemData;
			HeldItemData.HeldItemVisual = HeldItemVisual;
			HeldItemData.ItemReference = NewHeldItem;
			HeldItemData.SourceInventory = SourceInventory;
			HeldItemData.SourceSlotIndex = SourceItemSlotIndex;

			HeldItem->InitializeHeldItem(HeldItemData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: HeldItemVisual is nullptr!"), *FString(__FUNCTION__));
		}
	}
}

void UAWeekGameUIManager::HandleItemSlotShiftLeftClick(const FAWeekItemSlot& ClickedItemSlot) const
{

	if (!ClickedItemSlot.bIsEmpty && InventoryMainPanelWidget->IsChestOpen())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: execute"), *FString(__FUNCTION__));
		UAWeekInventoryComponent* SourceInventory = ClickedItemSlot.OwningInventory;
		UAWeekInventoryComponent* TargetInventory = nullptr;
		if (SourceInventory == InventoryMainPanelWidget->GetPlayerInventoryComponent())
		{
			TargetInventory = InventoryMainPanelWidget->GetChestInventoryComponent();
		}
		else
		{
			TargetInventory = InventoryMainPanelWidget->GetPlayerInventoryComponent();
		}
		
		if (SourceInventory && TargetInventory)
		{
			SourceInventory->TransferItem(ClickedItemSlot, TargetInventory);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Source Inventory or TargetInventory is null."), *FString(__FUNCTION__));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Chest is not open"), *FString(__FUNCTION__));
	}
}