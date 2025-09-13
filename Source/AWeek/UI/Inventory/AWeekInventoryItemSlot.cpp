// game
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/UI/Inventory/AWeekInventoryToolTip.h"
#include "AWeek/UI/Inventory/AWeekDragItemVisual.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Player/AWeekPlayerController.h"

// engine
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

const TObjectPtr<UAWeekItemBase> UAWeekInventoryItemSlot::GetItemReference() const
{
	return OwningInventory->GetItemSlotAt(ItemSlotIndex).Item;
}

void UAWeekInventoryItemSlot::InitializeItemSlot()
{
	if (ItemSlotIndex == -1) return;
	// set ToolTip
	if (OwningInventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OwningInventory is null!"), *FString(__FUNCTION__));
		return;

	}
	const FAWeekItemSlot& ItemSlot = OwningInventory->GetItemSlotAt(ItemSlotIndex);
	if (IsValid(ItemSlot.Item) && ToolTipClass)
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

	if (IsValid(ItemSlot.Item))
	{
		switch (ItemSlot.Item->ItemQuality)
		{
		case EAWeekItemQuality::Shoddy:
			ItemBorder->SetBrushColor(FLinearColor::Gray);
			break;
		case EAWeekItemQuality::Common:
			ItemBorder->SetBrushColor(FLinearColor::White);
			break;
		case EAWeekItemQuality::Quality:
			ItemBorder->SetBrushColor(FLinearColor(0.0f, 0.51f, 0.169f));
			break;
		case EAWeekItemQuality::Masterwork:
			ItemBorder->SetBrushColor(FLinearColor(0.0f, 0.4f, 0.75f));
			break;
		case EAWeekItemQuality::Grandmaster:
			ItemBorder->SetBrushColor(FLinearColor(1.0f, 0.45f, 0.0f)); // orange
			break;
		default:
			break;
		}

		ItemIcon->SetBrushFromTexture(ItemSlot.Item->AssetData.Icon);

		if (ItemSlot.Item->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemSlot.Item->Quantity));
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		ItemBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f));
	}

	AAWeekPlayerController* Controller = Cast<AAWeekPlayerController>(GetWorld()->GetFirstPlayerController());
	OnLeftClick.BindUObject(Controller, &AAWeekPlayerController::HandleItemSlotLeftClick);
	OnRightClick.BindUObject(Controller, &AAWeekPlayerController::HandleItemSlotRightClick);
	OnShiftLeftClick.BindUObject(Controller, &AAWeekPlayerController::HandleItemSlotShiftLeftClick);
}

void UAWeekInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UAWeekInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UAWeekInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (InMouseEvent.IsShiftDown())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: ItemSlotIndex = %d"), *FString(__FUNCTION__), ItemSlotIndex);

			const FAWeekItemSlot& ClickedItemSlot = OwningInventory->GetItemSlotAt(ItemSlotIndex);
			UE_LOG(LogTemp, Warning, TEXT("%s: ClickedItemIndex: %d"), *FString(__FUNCTION__), ClickedItemSlot.ItemSlotIndex);
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
	// submenu on right click will happen here

	return Reply.Unhandled();
}

void UAWeekInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UAWeekInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Warning, TEXT("item slot drag detected"));;

	// if empty slot, no dragging
	//if (ItemSlotReference && !ItemSlotReference->bIsEmpty && DragItemVisualClass)
	if (OwningInventory->IsValidItemSlotIndex(ItemSlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: ItemSlotIndex=%d is invalid"), *FString(__FUNCTION__), ItemSlotIndex);
		return;
	}
	const FAWeekItemSlot& ItemSlot = OwningInventory->GetItemSlotAt(ItemSlotIndex);
	if (!ItemSlot.bIsEmpty && DragItemVisualClass)
	{
		TObjectPtr<UAWeekDragItemVisual> DragVisual = CreateWidget<UAWeekDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(ItemSlot.Item->AssetData.Icon);

		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		ItemSlot.Item->NumericData.bIsStackable
			? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemSlot.Item->Quantity))
			: DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemSlot.Item->Quantity));
		UAWeekItemDragDropOperation* DragItemOperation = NewObject<UAWeekItemDragDropOperation>();
		//DragItemOperation->SourceItemSlot = ItemSlotReference;
		DragItemOperation->ItemSlotIndex = ItemSlotIndex;
		DragItemOperation->SourceInventory = OwningInventory;


		UE_LOG(LogTemp, Warning, TEXT("DragItemOperation SourceInventory: %s"), *DragItemOperation->SourceInventory->GetName());

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}
}

bool UAWeekInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UAWeekItemDragDropOperation* ItemDragDrop = Cast<UAWeekItemDragDropOperation>(InOperation);
	if (ItemDragDrop)
	{
		UE_LOG(LogTemp, Warning, TEXT("swap item"));
		// TODO: if each owning inventory of item are different
		//ItemDragDrop->SourceItemSlot->OwningInventory->SwapItemSlot(*ItemSlotReference, *ItemDragDrop->SourceItemSlot);
		OwningInventory->SwapItemSlotWith(ItemSlotIndex, ItemDragDrop->ItemSlotIndex, ItemDragDrop->SourceInventory);
		return true;
	}
	return false;
}