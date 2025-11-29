// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Controller/AWeekInventoryController.h"
#include "AWeek/UI/Inventory/AWeekHeldItem.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekPlayerInventoryComponent.h"
#include "AWeek/Player/AWeekPlayerController.h"

void UAWeekInventoryController::InitializeInventoryController(const TObjectPtr<UAWeekGameUIManager> InUIManager, TSubclassOf<UAWeekHeldItemVisual> InHeldItemVisualClass)
{
	UIManager = InUIManager;
	HeldItemVisualClass = InHeldItemVisualClass;
}

void UAWeekInventoryController::UpdateHeldItemPosition(FVector2D NewPosition)
{
	if (IsValid(HeldItem))
	{
		HeldItem->UpdateHeldVisualPosition(NewPosition);
	}
}

void UAWeekInventoryController::ReturnHeldItemToInventory()
{
	if (IsHoldingItem())
	{
		HeldItem->ReturnHeldItemToInventory();
		HeldItem = nullptr;
	}
}

void UAWeekInventoryController::HandleItemSlotLeftClick(int32 ClickedItemSlotIndex, const TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	// if isHolding item
	//		if heldItem == slotItem => add item to SlotItem as possible and hold remaining items
	//		else => put HeldItem and hold SlotItem
	// else
	//		hold SlotItem

	if (IsHoldingItem())
	{
		const FAWeekInventorySlotData& ClickedItemSlot = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
		if (ClickedItemSlot.IsEmpty())
		{
			OwningInventory->PlaceItemAt(HeldItem->ReleaseHeldItem(), ClickedItemSlotIndex);
			HeldItem->ClearHeldItem();
			HeldItem = nullptr;
		}
		else
		{
			UAWeekPlayerInventoryComponent* PlayerInventory = UIManager->GetPlayerCharacter()->GetPlayerInventoryComponent();
			// if target slot item ID is same as clicked item ID, merge item quantity as possible and hold remaining quantity of item 
			if (ClickedItemSlot.Item->GetID() == HeldItem->GetItem()->GetID())
			{
				MergeItem(ClickedItemSlotIndex, OwningInventory);
			}
			// if clicked item slot is trash can slot
			else if (OwningInventory == PlayerInventory && PlayerInventory
				&& ClickedItemSlotIndex == PlayerInventory->GetTrashCanSlotIndex())
			{
				PlayerInventory->ClearTrashCanSlot();
				PlayerInventory->SetTrashCanSlot(HeldItem->ReleaseHeldItem());
				HeldItem->ClearHeldItem();
				HeldItem = nullptr;
			}
			// put held item to target slot and hold target slot item
			else
			{
				UAWeekItemBase* ClickedItem = OwningInventory->ReleaseItemAt(ClickedItemSlotIndex);
				OwningInventory->PlaceItemAt(HeldItem->ReleaseHeldItem(), ClickedItemSlotIndex);
				HeldItem->SetItem(ClickedItem);
			}
		}
	}
	else
	{
		if (!OwningInventory->GetItemSlotAt(ClickedItemSlotIndex).IsEmpty())
		{
			UAWeekItemBase* NewHeldItem = OwningInventory->ReleaseItemAt(ClickedItemSlotIndex);
			CreateHeldItem(NewHeldItem, OwningInventory, ClickedItemSlotIndex);
		}
	}
}

void UAWeekInventoryController::HandleItemSlotRightClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	const FAWeekInventorySlotData& ClickedItemSlot = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
	if (IsHoldingItem())
	{
		if (!ClickedItemSlot.IsEmpty())
		{
			if (HeldItem->GetItem()->GetID() == ClickedItemSlot.Item->GetID())
			{
				OwningInventory->RemoveAmountOfItem(ClickedItemSlotIndex, 1);
				HeldItem->SetHeldItemQuantity(HeldItem->GetItem()->GetQuantity() + 1);
			}
		}
	}
	else
	{
		if (!ClickedItemSlot.IsEmpty())
		{
			// hold one item
			UAWeekItemBase* NewHeldItem = ClickedItemSlot.Item->CreateItemCopy();
			NewHeldItem->SetQuantity(1);
			CreateHeldItem(NewHeldItem, OwningInventory, ClickedItemSlotIndex);
			OwningInventory->RemoveAmountOfItem(ClickedItemSlotIndex, 1);
		}
	}
}

/* Merge held item to item slot */
void UAWeekInventoryController::MergeItem(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Merge Item"), *FString(__FUNCTION__));

	int32 HeldItemQuantity = HeldItem->GetItem()->GetQuantity();
	int32 ActualAmountAdded = OwningInventory->AddItemQuantityAt(ClickedItemSlotIndex, HeldItemQuantity);
	HeldItemQuantity -= ActualAmountAdded;
	HeldItem->SetHeldItemQuantity(HeldItemQuantity);
	if (HeldItem->IsEmpty())
	{
		HeldItem = nullptr;
	}
}

void UAWeekInventoryController::CreateHeldItem(TObjectPtr<UAWeekItemBase> InHeldItem, TObjectPtr<UAWeekInventoryComponent> SourceInventory, int32 SourceItemSlotIndex)
{
	if (HeldItemVisualClass)
	{
		UAWeekHeldItemVisual* HeldItemVisual = Cast<UAWeekHeldItemVisual>(
			CreateWidget(UIManager->GetPlayerController(),
			HeldItemVisualClass));
		HeldItem = NewObject<UAWeekHeldItem>(UIManager->GetPlayerController());

		if (HeldItemVisual)
		{
			FHeldItemData HeldItemData;
			HeldItemData.HeldItemVisual = HeldItemVisual;
			HeldItemData.Item = InHeldItem;
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

void UAWeekInventoryController::HandleItemSlotShiftLeftClick(const FAWeekInventorySlotData& ClickedItemSlot) const
{
	if (!ClickedItemSlot.IsEmpty() && UIManager->GetPlayerCharacter()->GetChestInventoryComponent())
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s: execute"), *FString(__FUNCTION__));
		UAWeekInventoryComponent* SourceInventory = ClickedItemSlot.OwningInventory;
		UAWeekInventoryComponent* TargetInventory = nullptr;
		if (SourceInventory == UIManager->GetPlayerCharacter()->GetPlayerInventoryComponent())
		{
			TargetInventory = UIManager->GetPlayerCharacter()->GetChestInventoryComponent();
		}
		else
		{
			TargetInventory = UIManager->GetPlayerCharacter()->GetPlayerInventoryComponent();
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

void UAWeekInventoryController::HandleItemSlotControlLeftClick(int32 ClickedItemSlotIndex, const TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	const FAWeekInventorySlotData& ClickedItemSlotData = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
	TObjectPtr<UAWeekPlayerInventoryComponent> PlayerInventory = UIManager->GetPlayerCharacter()->GetPlayerInventoryComponent();
	if (!PlayerInventory)
	{
		return;
	}
	if (!ClickedItemSlotData.IsEmpty())
	{
		UAWeekItemBase* Item = OwningInventory->ReleaseItemAt(ClickedItemSlotIndex);
		PlayerInventory->ClearTrashCanSlot();
		PlayerInventory->SetTrashCanSlot(Item);
	}
}
