// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"

// Sets default values for this component's properties
UAWeekInventoryComponent::UAWeekInventoryComponent()
{

}


void UAWeekInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// initialize slot
	InventoryContents.SetNum(InventorySlotsCapacity);
	if (NumCols == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NumCols is zero!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("InventoryContents: %d"), InventoryContents.Num());

	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		InventoryContents[i].Row = i / NumCols;
		InventoryContents[i].Col = i % NumCols;
		InventoryContents[i].OwningInventory = this;
	}
	OnInventoryUpdated.Broadcast();
}

UAWeekItemBase* UAWeekInventoryComponent::FindMatchingItem(UAWeekItemBase* ItemIn) const
{
	if (ItemIn)
	{
		for (const FItemSlot& ItemSlot : InventoryContents)
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
		if (const TArray<FItemSlot>::ElementType* Result =
			InventoryContents.FindByPredicate(
				[&ItemIn](const FItemSlot& ItemSlot)
		{
			return !ItemSlot.bIsEmpty && ItemSlot.Item->ID == ItemIn->ID;
		}))
		{
			return Result->Item;
		}
	}
	return nullptr;
}

FItemSlot* UAWeekInventoryComponent::FindNextPartialStack(UAWeekItemBase* ItemIn)
{
	if (TArray<FItemSlot>::ElementType* Result =
		InventoryContents.FindByPredicate(
			[&ItemIn](FItemSlot& ItemSlot)
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

void UAWeekInventoryComponent::ClearItemSlot(FItemSlot& ItemSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("Clear Item Slot"));
	ItemSlot.OwningInventory = nullptr;
	ItemSlot.Item = nullptr;
	ItemSlot.bIsEmpty = true;
	OnInventoryUpdated.Broadcast();
}

int32 UAWeekInventoryComponent::RemoveAmountOfItem(FItemSlot& ItemSlot, int32 DesiredAmountToRemove)
{
	UE_LOG(LogTemp, Warning, TEXT("RemoveAmountOfItem"));
	if (!ItemSlot.Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemoveAmountOfItem: Item is null!"));
		return 0;
	}
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemSlot.Item->Quantity);

	// bug : sometimes, when player drops item, the item reference of item slot is null


	InventoryTotalWeight -= ActualAmountToRemove * ItemSlot.Item->GetItemSingleWeight();
	SetItemQuantity(ItemSlot, ItemSlot.Item->Quantity - ActualAmountToRemove);


	OnInventoryUpdated.Broadcast();
	return ActualAmountToRemove;
}

int32 UAWeekInventoryComponent::RemoveAmountOfItem(const FItemSlot& ItemSlot, int32 DesiredAmountToRemove)
{
	int32 ItemSlotIndex = ItemSlot.Row * NumCols + ItemSlot.Col;
	UE_LOG(LogTemp, Warning, TEXT("Remove ItemSlotIndex: %d"), ItemSlotIndex);

	if (InventoryContents.IsValidIndex(ItemSlotIndex))
	{
		return RemoveAmountOfItem(InventoryContents[ItemSlotIndex], DesiredAmountToRemove);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSlotIndex is invalid: ItemSlotIndex = %d, ContentsSize: %d"),
			ItemSlotIndex, InventoryContents.Num());
	}
	return 0;
}

void UAWeekInventoryComponent::SplitExistingStack(FItemSlot& ItemSlot, const int32 AmountToSplit)
{
	if (GetEmptySlotsNum() > 0)
	{
		RemoveAmountOfItem(ItemSlot, AmountToSplit);
		AddNewItem(ItemSlot.Item, AmountToSplit);
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
	FItemSlot& ItemSlot = InventoryContents[ItemSlotIndex];
	FItemSlot& OtherItemSlot = OtherInventory->InventoryContents[OtherItemSlotIndex];
	int32 ItemWeight = ItemSlot.Item ? ItemSlot.Item->GetItemSingleWeight() * ItemSlot.Item->Quantity : 0;
	int32 OtherItemWeight = OtherItemSlot.Item ? OtherItemSlot.Item->GetItemSingleWeight() * OtherItemSlot.Item->Quantity : 0;
	InventoryTotalWeight += OtherItemWeight - ItemWeight;
	OtherInventory->InventoryTotalWeight += ItemWeight - OtherItemWeight;
	Swap(ItemSlot.Item, OtherItemSlot.Item);
	Swap(ItemSlot.bIsEmpty, OtherItemSlot.bIsEmpty);
	//Swap(ItemSlot.OwningInventory, OtherItemSlot.OwningInventory);
	OnInventoryUpdated.Broadcast();
	OtherInventory->OnInventoryUpdated.Broadcast();
}

int32 UAWeekInventoryComponent::GetEmptySlotsNum()
{
	int32 EmptyItemCount = 0;
	for (const FItemSlot& ItemSlot : InventoryContents)
	{
		if (ItemSlot.bIsEmpty)
		{
			EmptyItemCount++;
		}
	}
	return EmptyItemCount;
}

FItemAddResult UAWeekInventoryComponent::HandleNonStackableItems(UAWeekItemBase* InputItem)
{
	// check if in the input items has valid weight
	if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemStackWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item has invalid weight valid."), InputItem->TextData.Name));
	}

	// will the item wiehgt overflow weight capacity
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. Item would overflow weight limit."), InputItem->TextData.Name));
	}

	// adding one more item wold over flow slot capacity
	if (GetEmptySlotsNum() == 0)
	{
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->TextData.Name));
	}

	AddNewItem(InputItem, 1);
	return FItemAddResult::AddedAll(1, FText::Format(
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
	FItemSlot* ExistingItemStack = FindNextPartialStack(ItemIn);

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
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// otherwise, the full remainder of the stack can be added
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}
	// can only be reached if there is no existing stack and no extra capacity slots
	return 0;
}

FItemAddResult UAWeekInventoryComponent::HandleAddItem(UAWeekItemBase* InputItem)
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
			return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
				FText::FromString("Successfully added {0} {1} to the inventory."),
				InitialRequestedAddAmount,
				InputItem->TextData.Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Parial amount of {0} added to the inventory. Number added = {1}"),
				InputItem->TextData.Name,
				StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."),
				InputItem->TextData.Name));
		}
	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed"));
}

void UAWeekInventoryComponent::AddNewItem(UAWeekItemBase* Item, const int32 AmountToAdd)
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
	int32 FirstEmptyIndex = GetFirstEmptySlotIndex();
	if (FirstEmptyIndex < InventorySlotsCapacity)
	{
		//InventoryContents[FirstEmptyIndex] = NewItem;
		InventoryContents[FirstEmptyIndex].Item = NewItem;
		InventoryContents[FirstEmptyIndex].bIsEmpty = false;
		InventoryContents[FirstEmptyIndex].OwningInventory = this;

		InventoryTotalWeight += NewItem->GetItemStackWeight();
		OnInventoryUpdated.Broadcast();
	}
}

void UAWeekInventoryComponent::SetItemQuantity(FItemSlot& ItemSlot, const int32 Quantity)
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
	int32 i = 0;
	for (; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i].bIsEmpty)
			return i;
	}
	return i;
}

int32 UAWeekInventoryComponent::FindItemIndex(const UAWeekItemBase* ItemIn) const
{
	int32 i = 0;
	for (; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i].Item == ItemIn)
		{
			return i;
		}
	}
	return i;
}