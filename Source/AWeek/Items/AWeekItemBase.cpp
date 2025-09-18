#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"

UAWeekItemBase::UAWeekItemBase() : bIsCopy(false), bIsPickup(false)
{

}

void UAWeekItemBase::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

UAWeekItemBase* UAWeekItemBase::CreateItemCopy() const
{
	UAWeekItemBase* ItemCopy = NewObject<UAWeekItemBase>(StaticClass());

	ItemCopy->ID = this->ID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->AssetData = this->AssetData;

	ItemCopy->bIsCopy = true;

	return ItemCopy;
}

void UAWeekItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);

		//if (OwningInventory)
		//{
		//	if (Quantity <= 0)
		//	{
		//		OwningInventory->RemoveSingleInstanceOfItem(this);
		//	}
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("ItemBase OwningInventory was null (item may be pickup)."));
		//}
	}
}

void UAWeekItemBase::Use(AAWeekPlayerCharacter* Character)
{

}
