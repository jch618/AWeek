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
	if (IsValid(HeldItemData.Item))
	{
		HeldItemData.Item->SetQuantity(Quantity);
		UpdateHeldItem();
	}
}

void UAWeekHeldItem::SetItem(TObjectPtr<UAWeekItemBase> Item)
{
	if (IsValid(Item))
	{
		HeldItemData.Item = Item;
		UpdateHeldItem();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: NewHeldItemData.Item is not valid"), *FString(__FUNCTION__));
	}
}

UAWeekItemBase* UAWeekHeldItem::ReleaseHeldItem()
{
	UAWeekItemBase* Item = HeldItemData.Item;
	HeldItemData.Item = nullptr;
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
	if (HeldItemData.Item && HeldItemData.HeldItemVisual)
	{
		if (HeldItemData.Item->GetQuantity() == 0)
		{
			ClearHeldItem();
		}
		else
		{
			HeldItemData.HeldItemVisual->ItemIcon->SetBrushFromTexture(HeldItemData.Item->GetAssetData().Icon);

			//HeldItemVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

			HeldItemData.Item->GetNumericData().bIsStackable
				? HeldItemData.HeldItemVisual->ItemQuantity->SetText(FText::AsNumber(HeldItemData.Item->GetQuantity()))
				: HeldItemData.HeldItemVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

			HeldItemData.HeldItemVisual->ItemQuantity->SetText(FText::AsNumber(HeldItemData.Item->GetQuantity()));
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
	HeldItemData.Item = nullptr;
	HeldItemData.SourceInventory = nullptr;
	HeldItemData.SourceSlotIndex = -1;
}

void UAWeekHeldItem::ReturnHeldItemToInventory()
{
	const FAWeekInventorySlotData& ItemSlot = HeldItemData.SourceInventory->GetItemSlotAt(HeldItemData.SourceSlotIndex);
	if (ItemSlot.bIsEmpty)
	{
		HeldItemData.SourceInventory->PlaceItemAt(HeldItemData.Item, HeldItemData.SourceSlotIndex);
	}
	else if (ItemSlot.Item->GetID() == HeldItemData.Item->GetID())
	{
		HeldItemData.SourceInventory->AddItemQuantityAt(HeldItemData.SourceSlotIndex, HeldItemData.Item->GetQuantity());
	}
	ClearHeldItem();
}
