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
	
	InventoryComponent = InInventoryComponent;
	InventoryComponent->SetIsLinkedToInventoryPanel(true);
	
	InventoryComponent->OnInventoryUpdated.AddUObject(this, &UAWeekInventoryPanel::RefreshInventory);
	InventoryComponent->OnEncumberedStatusChanged.AddUObject(this, &UAWeekInventoryPanel::OnEncumberedStatusChanged);

	InitializeGridPanel();
	
	// update the panel text and display its contents
	UpdateInfoText();
	bIsLinkedToInventory = true;

	OnInventoryLinked();
	
	UE_LOG(LogTemp, Log, TEXT("%s: Input inventory %s successfully linked to %s."),
		*FString(__FUNCTION__), *InInventoryComponent->GetName(), *GetName());
}

void UAWeekInventoryPanel::UnlinkFromInventory()
{
	if (!InventoryComponent)
	{
		return;
	}
	InventoryComponent->SetIsLinkedToInventoryPanel(false);
	InventoryComponent->OnInventoryUpdated.RemoveAll(this);
	InventoryComponent->OnEncumberedStatusChanged.RemoveAll(this);

	InventoryComponent = nullptr;
	bIsLinkedToInventory = false;
}


void UAWeekInventoryPanel::InitializeGridPanel()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	for (int32 i = 0; i < InventoryComponent->GetSlotsCapacity(); i++)
	{
		UAWeekInventoryItemSlot* ItemSlot = CreateWidget<UAWeekInventoryItemSlot>(this, InventorySlotClass);
		ItemSlot->SetItemSlotIndex(i);
		ItemSlot->SetInventory(InventoryComponent);
		InventoryGridPanel->AddChildToUniformGrid(ItemSlot, i / NumCols, i % NumCols);
	}
}

void UAWeekInventoryPanel::OnInventoryLinked()
{
	RefreshInventory();
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
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (InventoryComponent)
	{
		for (int32 i = 0; i < InventoryGridPanel->GetChildrenCount(); i++)
		{
			if (UAWeekInventoryItemSlot* ItemSlot =
				Cast<UAWeekInventoryItemSlot>(InventoryGridPanel->GetChildAt(i)))
			{
				ItemSlot->InitializeInventoryItemSlot(
					InventoryComponent->GetItemSlotAt(ItemSlot->GetItemSlotIndex()).Item);
			}
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
