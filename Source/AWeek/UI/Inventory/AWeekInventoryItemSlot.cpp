// game
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/UI/Inventory/AWeekInventoryToolTip.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Player/AWeekUIController.h"

// engine
#include "AWeek/UI/AWeekGameUIManager.h"
#include "Components/Image.h"


const UAWeekItemBase* UAWeekInventoryItemSlot::GetItemReference() const
{
	return OwningInventory->GetItemSlotAt(ItemSlotIndex).Item;
}

void UAWeekInventoryItemSlot::InitializeInventoryItemSlot(TObjectPtr<UAWeekItemBase> ItemReference)
{
	Super::InitializeItemSlot(ItemReference);
	
	if (IsValid(ItemReference) && ToolTipClass)
	{
		UAWeekInventoryToolTip* ToolTip = CreateWidget<UAWeekInventoryToolTip>(this, ToolTipClass);
		if (IsValid(ToolTip))
		{
			ToolTip->InitializeToolTip(this);
			SetToolTip(ToolTip);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: ToolTip is invalid"), *FString(__FUNCTION__));
		}
	}


	AAWeekUIController* Controller = Cast<AAWeekUIController>(GetWorld()->GetFirstPlayerController());
	UAWeekGameUIManager* UIManager = GetGameInstance()->GetSubsystem<UAWeekGameUIManager>();
	OnLeftClick.BindUObject(UIManager, &UAWeekGameUIManager::HandleItemSlotLeftClick);
	OnRightClick.BindUObject(UIManager, &UAWeekGameUIManager::HandleItemSlotRightClick);
	OnShiftLeftClick.BindUObject(UIManager, &UAWeekGameUIManager::HandleItemSlotShiftLeftClick);
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
		OnLeftClick.ExecuteIfBound(ItemSlotIndex, OwningInventory);
		return FReply::Handled();

		//return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnRightClick.ExecuteIfBound(ItemSlotIndex, OwningInventory);
		return FReply::Handled();
	}
	return Reply.Unhandled();
}
