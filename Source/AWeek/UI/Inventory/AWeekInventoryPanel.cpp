// Fill out your copyright notice in the Description page of Project Settings.

// game
#include "AWeek/UI/Inventory/AWeekInventoryPanel.h"
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/Components/AWeekInventoryComponent.h"

#include "CommonUIExtensions.h"

// engine
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

UAWeekInventoryPanel::UAWeekInventoryPanel() : NumCols(5)
{
}

void UAWeekInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsLinkedToInventory = false;
}

void UAWeekInventoryPanel::LinkToInventory(const TObjectPtr<UAWeekInventoryComponent> InInventoryComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	
	if (!InInventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Did not receive a valid input inventory component!"),
			*FString(__FUNCTION__));
		return;
	}

	if (InventoryComponent == InInventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Inventory %s is already linked to inventory: %s"),
			*FString(__FUNCTION__),	*GetName(),	*InventoryComponent->GetName());
		return;
	}
	// if inventory is already connected with panel, unlink that 
	if (InventoryComponent)
	{
		UnlinkFromInventory();
	}
	InventoryComponent = InInventoryComponent;

	InitializeGridPanel();
	
	// update the panel text and display its contents
	UpdateInfoText();
	bIsLinkedToInventory = true;

	BindInventoryDelegates();

	RefreshInventory();
	
	UE_LOG(LogTemp, Log, TEXT("%s: Input inventory %s successfully linked to %s."),
		*FString(__FUNCTION__), *InInventoryComponent->GetName(), *GetName());
}

void UAWeekInventoryPanel::UnlinkFromInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (!InventoryComponent)
	{
		return;
	}
	// InventoryComponent->OnInventoryUpdated.RemoveAll(this);
	UnBindInventoryDelegates();

	InventoryComponent = nullptr;
	bIsLinkedToInventory = false;
}


void UAWeekInventoryPanel::InitializeGridPanel()
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	InventoryGridPanel->ClearChildren();
	InventoryItemSlots.Empty();
	for (int32 i = 0; i < InventoryComponent->GetSlotsCapacity(); i++)
	{
		UAWeekInventoryItemSlot* ItemSlot = CreateWidget<UAWeekInventoryItemSlot>(this, InventorySlotClass);
		ItemSlot->SetItemSlotIndex(i);
		ItemSlot->SetInventory(InventoryComponent);
		InventoryItemSlots.Add(ItemSlot);
		InventoryGridPanel->AddChildToUniformGrid(ItemSlot, i / NumCols, i % NumCols);
	}
}

void UAWeekInventoryPanel::BindInventoryDelegates()
{
	InventoryComponent->OnSlotUpdated.AddUObject(this, &UAWeekInventoryPanel::OnSlotUpdate);
	InventoryComponent->OnEncumberedStatusChanged.AddUObject(this, &UAWeekInventoryPanel::OnEncumberedStatusChanged);
}

void UAWeekInventoryPanel::UnBindInventoryDelegates()
{
	InventoryComponent->OnSlotUpdated.RemoveAll(this);
	InventoryComponent->OnEncumberedStatusChanged.RemoveAll(this);
}

void UAWeekInventoryPanel::OnSlotUpdate(const FAWeekInventorySlotData& SlotData)
{
	UE_LOG(LogTemp, Warning, TEXT("%s:Index=%d"), *FString(__FUNCTION__), SlotData.SlotIndex);
	InventoryItemSlots[SlotData.SlotIndex]->InitializeInventoryItemSlot(SlotData.Item);
	UpdateInfoText();
}

void UAWeekInventoryPanel::RefreshInventory()
{
	RefreshInventoryPanel();
	UpdateInfoText();
}

void UAWeekInventoryPanel::UpdateInfoText() const
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

void UAWeekInventoryPanel::RefreshInventoryPanel()
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (InventoryComponent)
	{
		for (int32 i = 0; i < InventoryItemSlots.Num(); i++)
		{
			InventoryItemSlots[i]->InitializeInventoryItemSlot(
				InventoryComponent->GetItemSlotAt(InventoryItemSlots[i]->GetItemSlotIndex()).Item);
		}
	}
}

void UAWeekInventoryPanel::OnEncumberedStatusChanged(bool bIsEncumbered) const
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
