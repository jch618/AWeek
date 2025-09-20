// game
#include "AWeek/UI/Inventory/AWeekHeldItem.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UAWeekHeldItem::UAWeekHeldItem() : HeldItemVisualXOffset(5.0), HeldItemVisualYOffset(5.0)
{
	
}

void UAWeekHeldItem::SetHeldItemQuantity(int32 Quantity)
{
	if (IsValid(HeldItemData.ItemReference))
	{
		HeldItemData.ItemReference->SetQuantity(Quantity);
		UpdateHeldItem();
	}
}

void UAWeekHeldItem::SetItemReference(TObjectPtr<UAWeekItemBase> NewItemReference)
{
	if (IsValid(NewItemReference))
	{
		HeldItemData.ItemReference = NewItemReference;
		UpdateHeldItem();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: NewHeldItemData.ItemReference is not valid"), *FString(__FUNCTION__));
	}
}

UAWeekItemBase* UAWeekHeldItem::ReleaseHeldItem()
{
	UAWeekItemBase* Item = HeldItemData.ItemReference;
	HeldItemData.ItemReference = nullptr;
	return Item;
}

void UAWeekHeldItem::InitializeHeldItem(const FHeldItemData& NewHeldItemData)
{
	HeldItemData = NewHeldItemData;
	if (NewHeldItemData.HeldItemVisual)
	{
		HeldItemData.HeldItemVisual->AddToViewport();
	}
	UpdateHeldItem();
}

void UAWeekHeldItem::UpdateHeldItem()
{
	if (HeldItemData.ItemReference && HeldItemData.HeldItemVisual)
	{
		if (HeldItemData.ItemReference->Quantity == 0)
		{
			ClearHeldItem();
		}
		else
		{
			HeldItemData.HeldItemVisual->ItemIcon->SetBrushFromTexture(HeldItemData.ItemReference->AssetData.Icon);

			//HeldItemVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

			HeldItemData.ItemReference->NumericData.bIsStackable
				? HeldItemData.HeldItemVisual->ItemQuantity->SetText(FText::AsNumber(HeldItemData.ItemReference->Quantity))
				: HeldItemData.HeldItemVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

			HeldItemData.HeldItemVisual->ItemQuantity->SetText(FText::AsNumber(HeldItemData.ItemReference->Quantity));
		}
	}
}

void UAWeekHeldItem::UpdateHeldVisualPosition(FVector2D MousePos)
{
	if (HeldItemData.HeldItemVisual)
	{
		MousePos.X += HeldItemVisualXOffset;
		MousePos.Y += HeldItemVisualYOffset;
		HeldItemData.HeldItemVisual->SetPositionInViewport(MousePos);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	}
}

void UAWeekHeldItem::ClearHeldItem()
{
	if (IsValid(HeldItemData.HeldItemVisual))
	{
		HeldItemData.HeldItemVisual->RemoveFromParent();
		HeldItemData.HeldItemVisual = nullptr;
	}
	HeldItemData.ItemReference = nullptr;
	HeldItemData.SourceInventory = nullptr;
	HeldItemData.SourceSlotIndex = -1;
}

void UAWeekHeldItem::ReturnHeldItemToInventory()
{
	const FAWeekItemSlot& ItemSlot = HeldItemData.SourceInventory->GetItemSlotAt(HeldItemData.SourceSlotIndex);
	if (ItemSlot.bIsEmpty)
	{
		HeldItemData.SourceInventory->PlaceItemAt(HeldItemData.ItemReference, HeldItemData.SourceSlotIndex);
	}
	else if (ItemSlot.Item->ID == HeldItemData.ItemReference->ID)
	{
		HeldItemData.SourceInventory->AddItemQuantityAt(HeldItemData.SourceSlotIndex, HeldItemData.ItemReference->Quantity);
	}
	ClearHeldItem();
}
