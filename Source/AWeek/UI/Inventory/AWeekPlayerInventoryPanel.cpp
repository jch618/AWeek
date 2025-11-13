// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekPlayerInventoryPanel.h"

#include "AWeekInventoryItemSlot.h"
#include "AWeekItemSlot.h"
#include "AWeek/Components/AWeekPlayerInventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UAWeekPlayerInventoryPanel::LinkToInventory(const TObjectPtr<UAWeekPlayerInventoryComponent> InPlayerInventoryComponent)
{
	PlayerInventoryComponent = InPlayerInventoryComponent;
	Super::LinkToInventory(InPlayerInventoryComponent);
}

void UAWeekPlayerInventoryPanel::InitializeGridPanel()
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	const int32 HotBarSize = PlayerInventoryComponent->GetHotBarInventorySize();
	const int32 InventorySize = InventoryComponent->GetSlotsCapacity();
	for (int32 i = 0; i < InventorySize; i++)
	{
		UAWeekInventoryItemSlot* ItemSlot = CreateWidget<UAWeekInventoryItemSlot>(this, InventorySlotClass);
		ItemSlot->SetInventory(InventoryComponent);
		ItemSlot->SetItemSlotIndex(i);
		if (i < HotBarSize)
		{
			HotBarGridPanel->AddChildToUniformGrid(ItemSlot, 0, i);
		}
		else if (i < InventorySize - 1)
		{
			const int32 InventoryGridIndex = i - HotBarSize; 
			InventoryGridPanel->AddChildToUniformGrid(ItemSlot,
				InventoryGridIndex / NumCols, InventoryGridIndex % NumCols);
		}
		else
		{
			// trash can
			InventoryGridPanel->AddChildToUniformGrid(ItemSlot, (InventorySize-1) / NumCols, NumCols);
		}
	}
}

void UAWeekPlayerInventoryPanel::RefreshInventoryPanel()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	Super::RefreshInventoryPanel();
	
	if (PlayerInventoryComponent)
	{
		for (int32 i = 0; i < HotBarGridPanel->GetChildrenCount(); i++)
		{
			if (UAWeekInventoryItemSlot* ItemSlot =
				Cast<UAWeekInventoryItemSlot>(HotBarGridPanel->GetChildAt(i)))
			{
				ItemSlot->InitializeInventoryItemSlot(
					PlayerInventoryComponent->GetItemSlotAt(ItemSlot->GetItemSlotIndex()).Item);
			}
		}
	}
}

