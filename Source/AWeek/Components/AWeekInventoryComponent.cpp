// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"

// Sets default values for this component's properties
UAWeekInventoryComponent::UAWeekInventoryComponent() : bIsLinkedToInventoryPanel(false)
{

}

void UAWeekInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// initialize slot
	check(NumCols > 0);

	for (int32 i = 0; i < InventorySlotsCapacity; i++)
	{
		InventoryContents.Add(FAWeekItemSlot(i, this));
	}
	OnInventoryUpdated.Broadcast();
}

UAWeekItemBase* UAWeekInventoryComponent::FindMatchingItem(UAWeekItemBase* ItemIn) const
{
	if (ItemIn)
	{
		for (const FAWeekItemSlot& ItemSlot : InventoryContents)
		{
			if (ItemSlot.Item == ItemIn)
			{
				return ItemSlot.Item;
			}
		}
	}
	return nullptr;
}

UAWeekItemBase* UAWeekInventoryComponent::FindNextItemByID(UAWeekItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<FAWeekItemSlot>::ElementType* Result =
			InventoryContents.FindByPredicate(
				[&ItemIn](const FAWeekItemSlot& ItemSlot)
		{
			return !ItemSlot.bIsEmpty && ItemSlot.Item->ID == ItemIn->ID;
		}))
		{
			return Result->Item;
		}
	}
	return nullptr;
}

FAWeekItemSlot* UAWeekInventoryComponent::FindNextPartialStack(UAWeekItemBase* ItemIn)
{
	if (TArray<FAWeekItemSlot>::ElementType* Result =
		InventoryContents.FindByPredicate(
			[&ItemIn](FAWeekItemSlot& ItemSlot)
	{
		return !ItemSlot.bIsEmpty && ItemSlot.Item->ID == ItemIn->ID && !ItemSlot.Item->IsFullItemStack();
	}))
	{
		return Result;
	}
	return nullptr;
}

int32 UAWeekInventoryComponent::CalculateWeightAddAmount(UAWeekItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UAWeekInventoryComponent::CalculateNumberForFullStack(UAWeekItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UAWeekInventoryComponent::ClearItemSlot(FAWeekItemSlot& ItemSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("Clear Item Slot"));
	ItemSlot.Item = nullptr;
	ItemSlot.bIsEmpty = true;
	OnInventoryUpdated.Broadcast();
}

int32 UAWeekInventoryComponent::RemoveAmountOfItem(FAWeekItemSlot& ItemSlot, int32 DesiredAmountToRemove)
{
	if (!ItemSlot.bIsEmpty && DesiredAmountToRemove > 0)
	{
		const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemSlot.Item->Quantity);

		InventoryTotalWeight -= ActualAmountToRemove * ItemSlot.Item->GetItemSingleWeight();
		SetItemQuantity(ItemSlot, ItemSlot.Item->Quantity - ActualAmountToRemove);

		OnInventoryUpdated.Broadcast();
		return ActualAmountToRemove;
	}
	return 0;
}

int32 UAWeekInventoryComponent::RemoveAmountOfItem(int32 TargetItemSlotIndex, int32 DesiredAmountToRemove)
{
	if (InventoryContents.IsValidIndex(TargetItemSlotIndex))
	{
		return RemoveAmountOfItem(InventoryContents[TargetItemSlotIndex], DesiredAmountToRemove);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: TargetItemSlotIndex = %d, ContentsSize: %d"),
			*FString(__FUNCTION__), TargetItemSlotIndex, InventoryContents.Num());
	}
	return 0;
}

UAWeekItemBase* UAWeekInventoryComponent::ReleaseItemAt(int32 TargetItemSlotIndex)
{
	if (IsValidItemSlotIndex(TargetItemSlotIndex) && !InventoryContents[TargetItemSlotIndex].bIsEmpty)
	{
		InventoryTotalWeight -= InventoryContents[TargetItemSlotIndex].Item->GetItemStackWeight();
		UAWeekItemBase* ResultItem = InventoryContents[TargetItemSlotIndex].Item;
		ClearItemSlot(InventoryContents[TargetItemSlotIndex]);
		return ResultItem;
	}
	return nullptr;
}

void UAWeekInventoryComponent::SplitExistingStack(FAWeekItemSlot& ItemSlot, const int32 AmountToSplit)
{
	if (GetEmptySlotsNum() > 0)
	{
		RemoveAmountOfItem(ItemSlot, AmountToSplit);
		AddNewItem(ItemSlot.Item, AmountToSplit, -1);
	}
}

void UAWeekInventoryComponent::SwapItemSlotWith(const int32 ItemSlotIndex, const int32 OtherItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OtherInventory)
{
	if (OtherInventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SwapItemSlotWith: OtherInventory is null!"));
		return;
	}
	if (!InventoryContents.IsValidIndex(ItemSlotIndex) ||
		!OtherInventory->InventoryContents.IsValidIndex(OtherItemSlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("SwapItemSlotWith: Invalid index: ItemSlotIndex: %d, OtherItemSlotIndex: %d."), ItemSlotIndex, OtherItemSlotIndex);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s: ItemSlotIndex: %d, OtherItemSlotIndex: %d"), *FString(__FUNCTION__), ItemSlotIndex, OtherItemSlotIndex);
	FAWeekItemSlot& ItemSlot = InventoryContents[ItemSlotIndex];
	FAWeekItemSlot& OtherItemSlot = OtherInventory->InventoryContents[OtherItemSlotIndex];
	int32 ItemWeight = ItemSlot.Item ? ItemSlot.Item->GetItemStackWeight() : 0;
	int32 OtherItemWeight = OtherItemSlot.Item ? OtherItemSlot.Item->GetItemStackWeight() : 0;
	if (CanAddItemWeight(OtherItemWeight - ItemWeight) && OtherInventory->CanAddItemWeight(ItemWeight - OtherItemWeight))
	{
		InventoryTotalWeight += OtherItemWeight - ItemWeight;
		OtherInventory->InventoryTotalWeight += ItemWeight - OtherItemWeight;
		Swap(ItemSlot.Item, OtherItemSlot.Item);
		Swap(ItemSlot.bIsEmpty, OtherItemSlot.bIsEmpty);
		OnInventoryUpdated.Broadcast();
		OtherInventory->OnInventoryUpdated.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Swap can't occurs."), *FString(__FUNCTION__));
	}
}

int32 UAWeekInventoryComponent::GetEmptySlotsNum() const
{
	int32 EmptyItemCount = 0;
	for (const FAWeekItemSlot& ItemSlot : InventoryContents)
	{
		if (ItemSlot.bIsEmpty)
		{
			EmptyItemCount++;
		}
	}
	return EmptyItemCount;
}

FAWeekItemAddResult UAWeekInventoryComponent::HandleNonStackableItems(UAWeekItemBase* InputItem)
{
	// check if in the input items has valid weight
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemStackWeight() < 0)
	{
		return FAWeekItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item has invalid weight valid."), InputItem->TextData.Name));
	}

	// will the item wiehgt overflow weight capacity
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FAWeekItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->TextData.Name));
	}

	// adding one more item wold over flow slot capacity
	if (GetEmptySlotsNum() == 0)
	{
		return FAWeekItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
	}

	AddNewItem(InputItem, 1, -1);
	return FAWeekItemAddResult::AddedAll(1, FText::Format(
		FText::FromString("Successfully added a single {0} to the inventory."), InputItem->TextData.Name));
}

int32 UAWeekInventoryComponent::HandleStackableItems(UAWeekItemBase* ItemIn, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(ItemIn->GetItemStackWeight()))
	{
		// invaild item data
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

	// check if the input item already exists in the inventory and is not a full stack
	FAWeekItemSlot* ExistingItemStack = FindNextPartialStack(ItemIn);

	// distribute item stack over existing stacks
	while (ExistingItemStack)
	{
		// calculate how many of the existing item would be needed to make the next full stack
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack->Item, AmountToDistribute);
		// calucate how many of the AmountToMakeStack can actually be carried based on weight capacity
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack->Item, AmountToMakeFullStack);

		// as long as the remaining amount of the item does not overflow weight capacity
		if (WeightLimitAddAmount > 0)
		{
			// adjust the existing itmes stack quantity and inventory total weight
			ExistingItemStack->Item->SetQuantity(ExistingItemStack->Item->Quantity + WeightLimitAddAmount);
			InventoryTotalWeight += ExistingItemStack->Item->GetItemSingleWeight() * WeightLimitAddAmount;

			// adjust the count to be distributed
			AmountToDistribute -= WeightLimitAddAmount;
			ItemIn->SetQuantity(AmountToDistribute);

			// if max weight capacity would be exeeded by another items, just return early
			if (InventoryTotalWeight + ExistingItemStack->Item->GetItemSingleWeight() > InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		else if (WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				// this block wil be reached if distributing an item across multiple stacks
				// and the weight limit is hit during that process
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			// all of the input item was distributed across existing stacks
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// check if there is still another vaild partial stack of the input item
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// no more partial stacks found, check if a new stack can be added
	if (GetEmptySlotsNum() > 0)
	{
		// attempt to add as many from the remaining itme quantity that can fit inventory weight capacity
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);
		if (WeightLimitAddAmount > 0)
		{
			// if there is still more to distribute, but weight limit has been reached
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				// create a copy since only a partial stack is begin added
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount, -1);
				return RequestedAddAmount - AmountToDistribute;
			}

			// otherwise, the full remainder of the stack can be added
			AddNewItem(ItemIn, AmountToDistribute, -1);
			return RequestedAddAmount;
		}
	}
	// can only be reached if there is no existing stack and no extra capacity slots
	return 0;
}

FAWeekItemAddResult UAWeekInventoryComponent::HandleAddItem(UAWeekItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// handle non-stackable items
		if (!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// handle stackable
		const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);
		UE_LOG(LogTemp, Warning, TEXT("StackableAmountAdded: %d"), StackableAmountAdded);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			return FAWeekItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FAWeekItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Parial amount of {0} added to the inventory. Number added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FAWeekItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."),
				InputItem->TextData.Name));
		}
	}
	check(false);
	return FAWeekItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed"));
}

void UAWeekInventoryComponent::AddNewItem(UAWeekItemBase* Item, const int32 AmountToAdd, int32 TargetIndex)
{
	// if multiple inventory(ex chest) refer same item, It would occur error
	UAWeekItemBase* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// if the item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// used when splitting or dragging to/from another inventory 
		NewItem = Item->CreateItemCopy();
	}

	NewItem->SetQuantity(AmountToAdd);

	UE_LOG(LogTemp, Warning, TEXT("add item to inventory"));
	TargetIndex = (TargetIndex == -1 ? GetFirstEmptySlotIndex() : TargetIndex);
	if (IsValidItemSlotIndex(TargetIndex))
	{
		//InventoryContents[FirstEmptyIndex] = NewItem;
		InventoryContents[TargetIndex].Item = NewItem;
		InventoryContents[TargetIndex].bIsEmpty = false;
		InventoryContents[TargetIndex].OwningInventory = this;

		InventoryTotalWeight += NewItem->GetItemStackWeight();
		OnInventoryUpdated.Broadcast();
	}
}


int32 UAWeekInventoryComponent::AddItemQuantityAt(int32 ItemSlotIndex, int32 DesiredAddAmount)
{
	if (InventoryContents.IsValidIndex(ItemSlotIndex))
	{
		UAWeekItemBase* TargetItem = InventoryContents[ItemSlotIndex].Item;
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(TargetItem, DesiredAddAmount);
		const int32 ActualAddAmount = CalculateWeightAddAmount(TargetItem, AmountToMakeFullStack);

		InventoryTotalWeight += ActualAddAmount * TargetItem->GetItemSingleWeight();
		TargetItem->SetQuantity(TargetItem->Quantity + ActualAddAmount);
		OnInventoryUpdated.Broadcast();
		return ActualAddAmount;
	}
	return 0;
}

void UAWeekInventoryComponent::PlaceItemAt(TObjectPtr<UAWeekItemBase> InputItem, int32 TargetIndex)
{
	//int32 DesiredAddAmount = InputItem->Quantity;
	/*const int32 WeightLimitAddAmount = CalculateWeightAddAmount(InputItem, DesiredAddAmount);*/
	AddNewItem(InputItem, InputItem->Quantity, TargetIndex);
	OnInventoryUpdated.Broadcast();
}

UAWeekItemBase* UAWeekInventoryComponent::TakeItemAt(int32 TargetIndex)
{
	if (IsValidItemSlotIndex(TargetIndex))
	{
		UAWeekItemBase* ResultItem = InventoryContents[TargetIndex].Item;
		ClearItemSlot(InventoryContents[TargetIndex]);
		return ResultItem;
	}
	return nullptr;
}

void UAWeekInventoryComponent::SetItemQuantity(FAWeekItemSlot& ItemSlot, const int32 Quantity)
{
	UE_LOG(LogTemp, Warning, TEXT("SetItemQuantity: Quantity = %d"), Quantity);
	ItemSlot.Item->SetQuantity(Quantity);
	if (Quantity <= 0)
	{
		ClearItemSlot(ItemSlot);
	}
}

int32 UAWeekInventoryComponent::GetFirstEmptySlotIndex()
{
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i].bIsEmpty)
			return i;
	}
	return -1;
}

int32 UAWeekInventoryComponent::FindItemIndex(const UAWeekItemBase* ItemIn) const
{
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i].Item == ItemIn)
		{
			return i;
		}
	}
	return -1;
}