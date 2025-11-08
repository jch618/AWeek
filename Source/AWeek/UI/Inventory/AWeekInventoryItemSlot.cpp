// game
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/UI/Inventory/AWeekInventoryToolTip.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
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

	UAWeekInventoryController* InventoryController = GetGameInstance()->GetSubsystem<UAWeekGameUIManager>()->GetInventoryController();
	OnLeftClick.BindUObject(InventoryController, &UAWeekInventoryController::HandleItemSlotLeftClick);
	OnRightClick.BindUObject(InventoryController, &UAWeekInventoryController::HandleItemSlotRightClick);
	OnShiftLeftClick.BindUObject(InventoryController, &UAWeekInventoryController::HandleItemSlotShiftLeftClick);
}

FReply UAWeekInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (InMouseEvent.IsShiftDown())
		{
			// UE_LOG(LogTemp, Warning, TEXT("%s: ItemSlotIndex = %d"), *FString(__FUNCTION__), ItemSlotIndex);
			const FAWeekInventorySlotData& ClickedItemSlot = OwningInventory->GetItemSlotAt(ItemSlotIndex);
			// UE_LOG(LogTemp, Warning, TEXT("%s: ClickedItemIndex: %d"), *FString(__FUNCTION__), ClickedItemSlot.ItemSlotIndex);
			OnShiftLeftClick.ExecuteIfBound(ClickedItemSlot);
			return FReply::Handled();
		}
		OnLeftClick.ExecuteIfBound(ItemSlotIndex, OwningInventory.Get());
		return FReply::Handled();

		//return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnRightClick.ExecuteIfBound(ItemSlotIndex, OwningInventory.Get());
		return FReply::Handled();
	}
	return Reply.Unhandled();
}
