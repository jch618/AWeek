#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/UI/Inventory/AWeekInventoryToolTip.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeek/UI/Controller/AWeekInventoryController.h"

// engine
#include "AWeek/UI/AWeekGameUIManager.h"
#include "Components/Image.h"


const UAWeekItemBase* UAWeekInventoryItemSlot::GetItem() const
{
	return OwningInventory->GetItemSlotAt(ItemSlotIndex).Item;
}

void UAWeekInventoryItemSlot::InitializeInventoryItemSlot(const TObjectPtr<UAWeekItemBase> Item)
{
	Super::InitializeItemSlot(Item);

	if (!OnLeftClick.IsBound())
	{
		UAWeekInventoryController* InventoryController = GetGameInstance()->GetSubsystem<UAWeekGameUIManager>()->GetInventoryController();
		OnLeftClick.BindUObject(InventoryController, &UAWeekInventoryController::HandleItemSlotLeftClick);
		OnRightClick.BindUObject(InventoryController, &UAWeekInventoryController::HandleItemSlotRightClick);
		OnShiftLeftClick.BindUObject(InventoryController, &UAWeekInventoryController::HandleItemSlotShiftLeftClick);
		OnControlLeftClick.BindUObject(InventoryController, &UAWeekInventoryController::HandleItemSlotControlLeftClick);
	}
}

FReply UAWeekInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (InMouseEvent.IsShiftDown())
		{
			const FAWeekInventorySlotData& ClickedItemSlot = OwningInventory->GetItemSlotAt(ItemSlotIndex);
			OnShiftLeftClick.ExecuteIfBound(ClickedItemSlot);
			return FReply::Handled();
		}
		if (InMouseEvent.IsControlDown())
		{
			OnControlLeftClick.ExecuteIfBound(ItemSlotIndex, OwningInventory.Get());
			return FReply::Handled();
		}
		OnLeftClick.ExecuteIfBound(ItemSlotIndex, OwningInventory.Get());
		return FReply::Handled();
	}
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnRightClick.ExecuteIfBound(ItemSlotIndex, OwningInventory.Get());
		return FReply::Handled();
	}
	return Reply.Unhandled();
}
