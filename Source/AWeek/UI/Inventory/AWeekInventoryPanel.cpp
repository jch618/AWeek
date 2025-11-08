// Fill out your copyright notice in the Description page of Project Settings.

// game
#include "AWeek/UI/Inventory/AWeekInventoryPanel.h"
#include "AWeek/UI/Inventory/AWeekInventoryMainPanel.h"
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"

#include "CommonUIExtensions.h"

// engine
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

UAWeekInventoryPanel::UAWeekInventoryPanel() : NumCols(5)
{
}

void UAWeekInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsLinkedToInventory = false;
}

void UAWeekInventoryPanel::LinkToInventory(const TObjectPtr<UAWeekInventoryComponent> InInventoryComponent, const TObjectPtr<AAWeekPlayerCharacter> InCharacter)
{
	if (InInventoryComponent)
	{
		// verify that the inventory reference is different from the incoming inventory
		if (InventoryComponent != InInventoryComponent)
		{
			InventoryComponent = InInventoryComponent;
			InventoryComponent->SetIsLinkedToInventoryPanel(true);
			
			InventoryComponent->OnInventoryUpdated.AddUObject(this, &UAWeekInventoryPanel::RefreshInventory);
			InventoryComponent->OnEncumberedStatusChanged.AddUObject(this, &UAWeekInventoryPanel::OnEncumeredStatusChanged);

			// update the panel text and display its contents
			SetInfoText();
			RefreshInventory();

			bIsLinkedToInventory = true;

			UE_LOG(LogTemp, Log, L"%s: Input inventory %s successfully linked to %s.",
				*FString(__FUNCTION__),
				*InInventoryComponent->GetName(),
				*GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				L"%s: Inventory %s is already linked to inventory: %s",
				*FString(__FUNCTION__),
				*GetName(),
				*InventoryComponent->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, L"%s: Did not receive a valid input inventory component!",
			*FString(__FUNCTION__));
	}
}

void UAWeekInventoryPanel::UnlinkFromInventory()
{
	InventoryComponent->SetIsLinkedToInventoryPanel(false);
	// removes all functions from the delegate's invocation list that are bound to the specified UserObject
	const uint8 DelegatesRemoved = InventoryComponent->OnInventoryUpdated.RemoveAll(this);
	if (DelegatesRemoved > 0)
	{
		UE_LOG(LogTemp, Warning, L"%s: %d InventoryWasUpdated delegates unbound from %s.",
			*FString(__FUNCTION__), DelegatesRemoved, *GetName());
	}

	InventoryComponent = nullptr;
	bIsLinkedToInventory = false;
}



void UAWeekInventoryPanel::RefreshInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (InventoryComponent && InventorySlotClass)
	{
		InventoryGridPanel->ClearChildren();

		const TArray<FAWeekInventorySlotData>& InventoryContents = InventoryComponent->GetInventoryContents();
		//UE_LOG(LogTemp, Warning, TEXT("%s: InventorySlots = %d"), *FString(__FUNCTION__), InventoryContents.Num());
		if (InventorySlotClass == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("InventorySlotClass is null!"));
			return;
		}
		for (int32 i = 0; i < InventoryContents.Num(); i++)
		{
			TObjectPtr<UAWeekInventoryItemSlot> ItemSlot = CreateWidget<UAWeekInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemSlotIndex(i);
			ItemSlot->SetInventory(InventoryComponent);
			ItemSlot->InitializeInventoryItemSlot(InventoryComponent->GetItemSlotAt(i).Item);

			UUniformGridSlot* GridSlot = InventoryGridPanel->AddChildToUniformGrid(ItemSlot,
				InventoryContents[i].ItemSlotIndex / NumCols,
				InventoryContents[i].ItemSlotIndex % NumCols);
		}
	}
	SetInfoText();
}

void UAWeekInventoryPanel::HandleShiftClickOnSlot(const FAWeekInventorySlotData& ClickedItemSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: detected"), *FString(__FUNCTION__));
	OnShiftClick.ExecuteIfBound(ClickedItemSlot);
}

void UAWeekInventoryPanel::SetInfoText() const
{
	const FString WeightInfoValue =
		FString::SanitizeFloat(InventoryComponent->GetInventoryTotalWeight()) + "/"
		+ FString::SanitizeFloat(InventoryComponent->GetWeightCapacity());

	const FString CapacityInfoValue =
		FString::FromInt(InventoryComponent->GetSlotsCapacity() - InventoryComponent->GetEmptySlotsNum()) + "/"
		+ FString::FromInt(InventoryComponent->GetSlotsCapacity());

	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}

bool UAWeekInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UAWeekItemDragDropOperation* ItemDragDrop = Cast<UAWeekItemDragDropOperation>(InOperation);
	if (!ItemDragDrop->SourceInventory->IsValidItemSlotIndex(ItemDragDrop->ItemSlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ItemSlotIndex: %d"), ItemDragDrop->ItemSlotIndex);
		return false;
	}
	const FAWeekInventorySlotData& ItemSlot = ItemDragDrop->SourceInventory->GetItemSlotAt(ItemDragDrop->ItemSlotIndex);

	if (!ItemSlot.bIsEmpty && InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel"));

		// returning true will stop the drop operation at this widget
		return true;
	}
	// returning false will cause the drop opertaion to fall through to underlying widgets (if any)
	return false;

}

void UAWeekInventoryPanel::OnEncumeredStatusChanged(bool bIsEncumbered) const
{
	if (bIsEncumbered)
	{
		WeightInfo->SetColorAndOpacity(FLinearColor::Red);
	}
	else
	{
		WeightInfo->SetColorAndOpacity(FLinearColor::White);
	}
}
