// Fill out your copyright notice in the Description page of Project Settings.

// game
#include "AWeek/UI/Inventory/AWeekInventoryPanel.h"
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"

// engine
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UAWeekInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsLinkedToInventory = false;
}

void UAWeekInventoryPanel::LinkToInventory(TObjectPtr<UAWeekInventoryComponent> InputInventory, TObjectPtr<AAWeekPlayerCharacter> InputCharacter)
{
	// InputCharacter is null by default, but if linking to a player then
	// create the submenu and link it to the input inventory
	//if (InputCharacter)
	//{
	//	if (InventorySubMenuClass)
	//	{
	//		SubMenu = CreateWidget<UInventorySubmenu>(this, InventorySubMenuClass);
	//		SubMenu->PlayerCharacter = InputCharacter;
	//		SubMenu->AddToViewport(6);
	//		SubMenu->SetVisibility(ESlateVisibility::Collapsed);
	//		SubMenu->LinkedInventory = InputInventory;
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, L"%s: InventorySubMenuClass was null!", *FString(__FUNCTION__));
	//	}
	//}

	if (InputCharacter)
	{
		InventoryTitle->SetText(FText::FromString("Player Inventory"));
	}
	else
	{
		InventoryTitle->SetText(FText::FromString("Chest Inventory"));
	}
	if (InputInventory)
	{
		// verify that the inventory reference is different from the incoming inventory
		if (this->InventoryReference != InputInventory)
		{
			this->InventoryReference = InputInventory;
			// bind the delegate so that changes in the linked inventory call RefreshInventory
			this->InventoryReference->OnInventoryUpdated.AddUObject(this, &UAWeekInventoryPanel::RefreshInventory);

			// update the panel text and display its contents
			SetInfoText();
			RefreshInventory();

			bIsLinkedToInventory = true;

			UE_LOG(LogTemp, Log, L"%s: Input inventory %s successfully linked to %s.",
				*FString(__FUNCTION__),
				*InputInventory->GetName(),
				*GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				L"%s: Inventory %s is already linked to inventory: %s",
				*FString(__FUNCTION__),
				*GetName(),
				*InventoryReference->GetName());
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
	// removes all functions from the delegate's invocation list that are bound to the specified UserObject
	const uint8 DelegatesRemoved = InventoryReference->OnInventoryUpdated.RemoveAll(this);
	if (DelegatesRemoved > 0)
	{
		UE_LOG(LogTemp, Warning, L"%s: %d InventoryWasUpdated delegates unbound from %s.",
			*FString(__FUNCTION__), DelegatesRemoved, *GetName());
	}

	InventoryReference = nullptr;
	//if (IsValid(SubMenu))
	//{
	//	SubMenu->BeginDestroy();
	//}
	bIsLinkedToInventory = false;
}


void UAWeekInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryGridPanel->ClearChildren();

		//TArray<UItemBase*> InventoryContents = InventoryReference->GetInventoryContents();
		TArray<FItemSlot>& InventoryContents = InventoryReference->GetInventoryContents();
		UE_LOG(LogTemp, Warning, TEXT("Refresh Inventory: InventorySlots = %d"), InventoryContents.Num());
		for (int32 i = 0; i < InventoryContents.Num(); i++)
		{
			TObjectPtr<UAWeekInventoryItemSlot> ItemSlot = CreateWidget<UAWeekInventoryItemSlot>(this, InventorySlotClass);
			//ItemSlot->SetItemReference(InventoryContents[i]);
			//ItemSlot->SetItemSlotReference(&InventoryContents[i]);
			ItemSlot->SetItemSlotIndex(i);
			ItemSlot->SetInventory(InventoryReference);
			//UE_LOG(LogTemp, Warning, TEXT("InventoryContents[%d]: Row=%d, Col=%d"), i, 
				//InventoryContents[i].Row, InventoryContents[i].Col);

			UUniformGridSlot* GridSlot = InventoryGridPanel->AddChildToUniformGrid(ItemSlot,
				InventoryContents[i].Row,
				InventoryContents[i].Col);
		}
	}
	SetInfoText();
}

void UAWeekInventoryPanel::SetInfoText() const
{
	const FString WeightInfoValue =
		FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/"
		+ FString::SanitizeFloat(InventoryReference->GetWeightCapacity());

	const FString CapacityInfoValue =
		FString::FromInt(InventoryReference->GetSlotsCapacity() - InventoryReference->GetEmptySlotsNum()) + "/"
		+ FString::FromInt(InventoryReference->GetSlotsCapacity());

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
	const FItemSlot& ItemSlot = ItemDragDrop->SourceInventory->GetItemSlotAt(ItemDragDrop->ItemSlotIndex);

	if (!ItemSlot.bIsEmpty && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel"));

		// returning true will stop the drop operation at this widget
		return true;
	}
	// returning false will cause the drop opertaion to fall through to underlying widgets (if any)
	return false;

}
