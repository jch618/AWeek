#include "AWeek/Items/AWeekItemBase.h"

#include "ContentBrowserDataSource.h"
#include "AWeek/Components/AWeekInventoryComponent.h"

UAWeekItemBase::UAWeekItemBase() : Quantity(0), bIsCopy(false), bIsPickup(false)
{

}

void UAWeekItemBase::InitializeFromItemData(const FAWeekItemData& InItemData, int32 InQuantity)
{
	ItemData = InItemData;
	UE_LOG(LogTemp, Warning, TEXT("%s: ID: %s, Weight: %f"), *FString(__FUNCTION__), *ItemData.ID.ToString(), ItemData.NumericData.Weight);

	ItemData.NumericData.bIsStackable = ItemData.NumericData.MaxStackSize > 1;
	Quantity = FMath::Max(1, InQuantity);
}

void UAWeekItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UAWeekItemBase* UAWeekItemBase::CreateFromData(const FAWeekItemData& InItemData, int32 InQuantity, UObject* Outer)
{
	if (!Outer)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Creating Item fails. Outer is null."), *FString(__FUNCTION__));
		return nullptr;
	}

	UAWeekItemBase* NewItem = NewObject<UAWeekItemBase>(Outer, UAWeekItemBase::StaticClass());
	NewItem->InitializeFromItemData(InItemData, InQuantity);
	return NewItem;
}

UAWeekItemBase* UAWeekItemBase::CreateFromRowHandle(const FDataTableRowHandle& RowHandle, int32 InQuantity, UObject* Outer)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	const FAWeekItemData* ItemData = RowHandle.GetRow<FAWeekItemData>(RowHandle.RowName.ToString());
	if (!ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to get item data from row: %s"),
			*FString(__FUNCTION__), *RowHandle.RowName.ToString());
		return nullptr;
	}
	UE_LOG(LogTemp, Error, TEXT("%s: Success to get item data from row: %s"),
		*FString(__FUNCTION__), *RowHandle.RowName.ToString());
	return CreateFromData(*ItemData, InQuantity, Outer);
}

UAWeekItemBase* UAWeekItemBase::CreateItemCopy() const
{
	UAWeekItemBase* ItemCopy = NewObject<UAWeekItemBase>(StaticClass());

	ItemCopy->ItemData = ItemData;
	ItemCopy->Quantity = this->Quantity;
	// ItemCopy->ItemData.ID = ItemData.ID;
	ItemCopy->bIsCopy = true;
	ItemCopy->bIsPickup = false;
	
	return ItemCopy;
}

void UAWeekItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0,
			ItemData.NumericData.bIsStackable ? ItemData.NumericData.MaxStackSize : 1);
	}
}

void UAWeekItemBase::Use(AAWeekPlayerCharacter* Character)
{

}
