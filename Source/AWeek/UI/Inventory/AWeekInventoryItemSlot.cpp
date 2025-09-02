// engine
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// game
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/UI/Inventory/AWeekInventoryToolTip.h"
#include "AWeek/UI/Inventory/AWeekDragItemVisual.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"

const TObjectPtr<UAWeekItemBase> UAWeekInventoryItemSlot::GetItemReference() const
{
	return OwningInventory->GetItemSlotAt(ItemSlotIndex).Item;
}

void UAWeekInventoryItemSlot::InitializeItemSlot()
{
	if (ItemSlotIndex == -1) return;
	// set ToolTip
	//if (!ItemSlotReference->bIsEmpty && ToolTipClass)
	if (OwningInventory == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item slot: OwningInventory is null!"));
		return;

	}
	const FAWeekItemSlot& ItemSlot = OwningInventory->GetItemSlotAt(ItemSlotIndex);
	if (!ItemSlot.bIsEmpty && ToolTipClass)
	{
		UAWeekInventoryToolTip* ToolTip = CreateWidget<UAWeekInventoryToolTip>(this, ToolTipClass);
		ToolTip->InitializeToolTip(this);
		SetToolTip(ToolTip);
	}

	const UAWeekItemBase* ItemReference = ItemSlot.Item;
	if (ItemReference)
	{
		switch (ItemReference->ItemQuality)
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

		ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		if (ItemReference->NumericData.bIsStackable)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
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
}

void UAWeekInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UAWeekInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	//if (ItemSlotIndex == -1) return;
	//// set ToolTip
	////if (!ItemSlotReference->bIsEmpty && ToolTipClass)
	//if (OwningInventory == nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Item slot: OwningInventory is null!"));
	//	return;

	//}
	//const FAWeekItemSlot& ItemSlot = OwningInventory->GetItemSlotAt(ItemSlotIndex);
	//if (!ItemSlot.bIsEmpty && ToolTipClass)
	//{
	//	UAWeekInventoryToolTip* ToolTip = CreateWidget<UAWeekInventoryToolTip>(this, ToolTipClass);
	//	ToolTip->InventorySlotBeingHovered = this;
	//	SetToolTip(ToolTip);
	//}

	//const UAWeekItemBase* ItemReference = ItemSlot.Item;
	//if (ItemReference)
	//{
	//	switch (ItemReference->ItemQuality)
	//	{
	//	case EAWeekItemQuality::Shoddy:
	//		ItemBorder->SetBrushColor(FLinearColor::Gray);
	//		break;
	//	case EAWeekItemQuality::Common:
	//		ItemBorder->SetBrushColor(FLinearColor::White);
	//		break;
	//	case EAWeekItemQuality::Quality:
	//		ItemBorder->SetBrushColor(FLinearColor(0.0f, 0.51f, 0.169f));
	//		break;
	//	case EAWeekItemQuality::Masterwork:
	//		ItemBorder->SetBrushColor(FLinearColor(0.0f, 0.4f, 0.75f));
	//		break;
	//	case EAWeekItemQuality::Grandmaster:
	//		ItemBorder->SetBrushColor(FLinearColor(1.0f, 0.45f, 0.0f)); // orange
	//		break;
	//	default:
	//		break;
	//	}

	//	ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

	//	if (ItemReference->NumericData.bIsStackable)
	//	{
	//		ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
	//	}
	//	else
	//	{
	//		ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//}
	//else
	//{
	//	ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
	//	ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	//	ItemBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f));
	//}
}

FReply UAWeekInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
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
	if (ItemSlotIndex == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSlotIndex is -1!"));
		return;
	}
	const FAWeekItemSlot& ItemSlot = OwningInventory->GetItemSlotAt(ItemSlotIndex);
	if (ItemSlotIndex != -1 && !ItemSlot.bIsEmpty && DragItemVisualClass)
	{
		const UAWeekItemBase* ItemReference = ItemSlot.Item;
		TObjectPtr<UAWeekDragItemVisual> DragVisual = CreateWidget<UAWeekDragItemVisual>(this, DragItemVisualClass);
		if (!DragVisual)
		{
			UE_LOG(LogTemp, Warning, TEXT("drag visual is null"));;
			return;
		}
		if (!DragVisual->ItemIcon)
		{
			UE_LOG(LogTemp, Warning, TEXT("item icon is null"));;
			return;
		}
		if (!ItemReference)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventoryItemSlot: Item reference is null!"));;
			return;
		}
		if (!ItemReference->AssetData.Icon)
		{
			UE_LOG(LogTemp, Warning, TEXT("asset data is null"));;
			return;
		}
		DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);

		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		ItemReference->NumericData.bIsStackable
			? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity))
			: DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

		DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
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