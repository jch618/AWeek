// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "GeometryCollection/GeometryCollectionParticlesData.h"

// Sets default values for this component's properties
UAWeekInventoryComponent::UAWeekInventoryComponent()
	: InventoryTotalWeight(0), InventorySlotsCapacity(18), InventoryWeightCapacity(30.0f)
{
}

void UAWeekInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventoryContents.Reserve(InventorySlotsCapacity);
	for (int32 i = 0; i < InventorySlotsCapacity; i++)
	{
		InventoryContents.Add(FAWeekInventorySlotData(i, this));
	}
}

UAWeekItemBase* UAWeekInventoryComponent::FindMatchingItem(UAWeekItemBase* InItem) const
{
	if (InItem)
	{
		for (const FAWeekInventorySlotData& ItemSlot : InventoryContents)
		{
			if (ItemSlot.Item == InItem)
			{
				return ItemSlot.Item;
			}
		}
	}
	return nullptr;
}

int32 UAWeekInventoryComponent::FindNextItemByID(FName ItemID) const
{
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		if (IsValid(InventoryContents[i].Item) && InventoryContents[i].Item->GetID() == ItemID)
		{
			return i;
		}
	}
	return -1;
}

int32 UAWeekInventoryComponent::FindNextPartialStack(FName ItemID)
{
	return InventoryContents.IndexOfByPredicate([ItemID](const FAWeekInventorySlotData& ItemSlot)
	{
		return !ItemSlot.IsEmpty()
				&& ItemSlot.Item->GetID() == ItemID
				&& !ItemSlot.Item->IsFullItemStack();
	});
}

int32 UAWeekInventoryComponent::CalculateWeightAddAmount(UAWeekItemBase* InItem, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / InItem->GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UAWeekInventoryComponent::CalculateNumberForFullStack(UAWeekItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->GetNumericData().MaxStackSize - StackableItem->GetQuantity();

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UAWeekInventoryComponent::ClearItemSlot(FAWeekInventorySlotData& ItemSlot)
{
	ItemSlot.Item = nullptr;
	// OnInventoryUpdated.Broadcast();
	UE_LOG(LogTemp, Display, TEXT("Clearing item slot"));
	OnSlotUpdated.Broadcast(ItemSlot);
}

int32 UAWeekInventoryComponent::RemoveAmountOfItem(FAWeekInventorySlotData& ItemSlot, int32 DesiredAmountToRemove)
{
	if (!ItemSlot.IsEmpty() && DesiredAmountToRemove > 0)
	{
		const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove, ItemSlot.Item->GetQuantity());

		UpdateInventoryTotalWeight(-1 * ActualAmountToRemove * ItemSlot.Item->GetItemSingleWeight());
		SetItemQuantity(ItemSlot, ItemSlot.Item->GetQuantity() - ActualAmountToRemove);

		OnSlotUpdated.Broadcast(ItemSlot);
		
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
	return 0;
}

bool UAWeekInventoryComponent::TryRemoveAmountOfItem(FName ItemID, int32 DesiredAmountToRemove)
{
	int32 AmountToRemove = DesiredAmountToRemove;
	while (AmountToRemove > 0)
	{
		int32 TargetIndex = FindNextItemByID(ItemID);
		if (InventoryContents.IsValidIndex(TargetIndex))
		{
			AmountToRemove -= RemoveAmountOfItem(InventoryContents[TargetIndex], AmountToRemove);
		}
		else
		{
			return false;
		}
	}
	return true;
}

UAWeekItemBase* UAWeekInventoryComponent::ReleaseItemAt(int32 TargetItemSlotIndex)
{
	if (IsValidItemSlotIndex(TargetItemSlotIndex) && !InventoryContents[TargetItemSlotIndex].IsEmpty())
	{
		UpdateInventoryTotalWeight(-InventoryContents[TargetItemSlotIndex].Item->GetItemStackWeight());
		UAWeekItemBase* ResultItem = InventoryContents[TargetItemSlotIndex].Item;
		ClearItemSlot(InventoryContents[TargetItemSlotIndex]);
		return ResultItem;
	}
	return nullptr;
}

void UAWeekInventoryComponent::SplitExistingStack(FAWeekInventorySlotData& ItemSlot, const int32 AmountToSplit)
{
	if (GetEmptySlotsNum() > 0)
	{
		RemoveAmountOfItem(ItemSlot, AmountToSplit);
		AddNewItem(ItemSlot.Item, AmountToSplit, -1);
	}
}

int32 UAWeekInventoryComponent::GetEmptySlotsNum() const
{
	int32 EmptyItemCount = 0;
	for (const FAWeekInventorySlotData& ItemSlot : InventoryContents)
	{
		if (ItemSlot.IsEmpty())
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
			FText::FromString("Could not add {0} to the inventory. Item has invalid weight valid."), InputItem->GetTextData().Name));
	}

	// adding one more item would overflow slot capacity
	if (GetEmptySlotsNum() == 0)
	{
		return FAWeekItemAddResult::AddedNone(FText::Format(
			FText::FromString("Could not add {0} to the inventory. All inventory slots are full."), InputItem->GetTextData().Name));
	}

	AddNewItem(InputItem, 1, -1);
	return FAWeekItemAddResult::AddedAll(1, FText::Format(
		FText::FromString("Successfully added a single {0} to the inventory."), InputItem->GetTextData().Name));
}

int32 UAWeekInventoryComponent::HandleStackableItems(UAWeekItemBase* InItem, int32 RequestedAddAmount)
{
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(InItem->GetItemStackWeight()))
	{
		// invaild item data
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;

	// check if the input item already exists in the inventory and is not a full stack
	int32 ExistingItemStackIndex = FindNextPartialStack(InItem->GetID());

	// distribute item stack over existing stacks
	while (InventoryContents.IsValidIndex(ExistingItemStackIndex))
	{
		const FAWeekInventorySlotData& ExistingItemStack = InventoryContents[ExistingItemStackIndex];
		
		// calculate how many of the existing item would be needed to make the next full stack
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack.Item, AmountToDistribute);

		// as long as the remaining amount of the item does not overflow weight capacity
		if (AmountToMakeFullStack > 0)
		{
			// adjust the existing itmes stack quantity and inventory total weight
			ExistingItemStack.Item->SetQuantity(ExistingItemStack.Item->GetQuantity() + AmountToMakeFullStack);
			UpdateInventoryTotalWeight(ExistingItemStack.Item->GetItemSingleWeight() * AmountToMakeFullStack);
			OnSlotUpdated.Broadcast(ExistingItemStack);

			// adjust the count to be distributed
			AmountToDistribute -= AmountToMakeFullStack;
			InItem->SetQuantity(AmountToDistribute);
		}
		else if (AmountToMakeFullStack <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				// this block wil be reached if distributing an item across multiple stacks
				// and the weight limit is hit during that process				
				return RequestedAddAmount - AmountToDistribute;
			}			
			return 0;
		}

		if (AmountToDistribute <= 0)
		{
			// all of the input item was distributed across existing stacks
			return RequestedAddAmount;
		}

		// check if there is still another vaild partial stack of the input item
		ExistingItemStackIndex = FindNextPartialStack(InItem->GetID());
	}

	// no more partial stacks found, check if a new stack can be added
	if (GetEmptySlotsNum() > 0)
	{
		// attempt to add as many from the remaining itme quantity that can fit inventory weight capacity
		if (AmountToDistribute > 0)
		{
			// otherwise, the full remainder of the stack can be added
			AddNewItem(InItem, AmountToDistribute, -1);
			return RequestedAddAmount;
		}
	}
	// can only be reached if there is no existing stack and no extra capacity slots
	return RequestedAddAmount - AmountToDistribute;
}

bool UAWeekInventoryComponent::CanAddItem(const FName ItemID, const int32 ItemSingleWeight, const int32 Quantity) const
{
	int32 QuantityToDistribute = Quantity;
	for (const FAWeekInventorySlotData& SlotData : InventoryContents)
	{
		if (SlotData.IsEmpty())
		{
			return true;
		}
		if (SlotData.Item->GetID() == ItemID)
		{
			QuantityToDistribute -= SlotData.Item->GetNumericData().MaxStackSize - SlotData.Item->GetQuantity();
		}
	}
	return QuantityToDistribute <= 0;
}

FAWeekItemAddResult UAWeekInventoryComponent::HandleAddItem(UAWeekItemBase* InputItem)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->GetQuantity();

		// handle non-stackable items
		if (!InputItem->GetNumericData().bIsStackable)
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
				InputItem->GetTextData().Name));
		}

		if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
		{
			return FAWeekItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
				FText::FromString("Partial amount of {0} added to the inventory. Number added = {1}"),
				InputItem->GetTextData().Name,
				StackableAmountAdded));
		}

		if (StackableAmountAdded <= 0)
		{
			return FAWeekItemAddResult::AddedNone(FText::Format(
				FText::FromString("Couldn't add {0} to the inventory. No remaining inventory slots, or invalid item."),
				InputItem->GetTextData().Name));
		}
	}
	check(false);
	return FAWeekItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed"));
}

// Add New item to Inventory and return TargetIndex
int UAWeekInventoryComponent::AddNewItem(UAWeekItemBase* Item, const int32 AmountToAdd, int32 TargetIndex)
{
	// if multiple inventory(ex chest) refer same item, It would occur error
	UAWeekItemBase* NewItem;

	if (Item->IsCopy() || Item->IsPickup())
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
	
	TargetIndex = (TargetIndex == -1 ? FindFirstEmptySlot() : TargetIndex);
	if (IsValidItemSlotIndex(TargetIndex))
	{
		InventoryContents[TargetIndex].Item = NewItem;
		InventoryContents[TargetIndex].OwningInventory = this;
		
		UpdateInventoryTotalWeight(NewItem->GetItemStackWeight());
		
		OnSlotUpdated.Broadcast(InventoryContents[TargetIndex]);
	}
	return TargetIndex;
}


int32 UAWeekInventoryComponent::AddItemQuantityAt(int32 ItemSlotIndex, int32 DesiredAddAmount)
{
	if (InventoryContents.IsValidIndex(ItemSlotIndex))
	{
		UAWeekItemBase* TargetItem = InventoryContents[ItemSlotIndex].Item;
		const int32 ActualAddAmount = CalculateNumberForFullStack(TargetItem, DesiredAddAmount);
		
		UpdateInventoryTotalWeight(ActualAddAmount * TargetItem->GetItemSingleWeight());
		
		TargetItem->SetQuantity(TargetItem->GetQuantity() + ActualAddAmount);
		
		// OnInventoryUpdated.Broadcast();
		OnSlotUpdated.Broadcast(InventoryContents[ItemSlotIndex]);
		
		return ActualAddAmount;
	}
	return 0;
}

void UAWeekInventoryComponent::PlaceItemAt(TObjectPtr<UAWeekItemBase> InputItem, int32 TargetIndex)
{
	AddNewItem(InputItem, InputItem->GetQuantity(), TargetIndex);
}

void UAWeekInventoryComponent::TransferItem(const FAWeekInventorySlotData& FromItemSlot, TObjectPtr<UAWeekInventoryComponent> TargetInventory)
{
	int32 DesiredAddAmount = FromItemSlot.Item->GetQuantity();
	FAWeekItemAddResult AddResult = TargetInventory->HandleAddItem(FromItemSlot.Item);
	if (AddResult.ActualAmountAdded == DesiredAddAmount)
	{
		// item pointer is moved to target inventory or item quantity is 0
		ReleaseItemAt(FromItemSlot.SlotIndex);
	}
	else
	{
		// clicked item quantity changed
		OnSlotUpdated.Broadcast(FromItemSlot);
	}
}

TMap<FName, int32> UAWeekInventoryComponent::GetInventoryItemCounts() const
{
	TMap<FName, int32> InventoryItemCounts;
	for (const FAWeekInventorySlotData& ItemSlot : InventoryContents)
	{
		if (!ItemSlot.IsEmpty())
		{
			if (InventoryItemCounts.Contains(ItemSlot.Item->GetID()))
			{
				InventoryItemCounts[ItemSlot.Item->GetID()] += ItemSlot.Item->GetQuantity();
			}
			else
			{
				InventoryItemCounts.Add(ItemSlot.Item->GetID(), ItemSlot.Item->GetQuantity());
			}
			
		}
	}
	return InventoryItemCounts;
}

void UAWeekInventoryComponent::SetItemQuantity(FAWeekInventorySlotData& ItemSlot, const int32 Quantity)
{
	ItemSlot.Item->SetQuantity(Quantity);
	if (Quantity <= 0)
	{
		ClearItemSlot(ItemSlot);
	}
}

int32 UAWeekInventoryComponent::FindFirstEmptySlot()
{
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i].IsEmpty())
			return i;
	}
	return -1;
}

void UAWeekInventoryComponent::UpdateInventoryTotalWeight(float DeltaWeight)
{
	bool bIsEncumbered = InventoryTotalWeight > InventoryWeightCapacity;
	InventoryTotalWeight += DeltaWeight;
	bool bNewIsEncumbered =  InventoryTotalWeight > InventoryWeightCapacity;
	if (bNewIsEncumbered != bIsEncumbered)
	{
		bIsEncumbered = bNewIsEncumbered;
		OnEncumberedStatusChanged.Broadcast(bIsEncumbered);
	}
}
