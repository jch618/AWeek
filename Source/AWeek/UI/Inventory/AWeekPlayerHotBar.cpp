// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekPlayerHotBar.h"

#include "AWeekItemSlot.h"
#include "AWeek/Components/AWeekPlayerInventoryComponent.h"

#include "Components/UniformGridPanel.h"

void UAWeekPlayerHotBar::InitializeHotBar(const TObjectPtr<UAWeekPlayerInventoryComponent> InPlayerInventoryComponent)
{
	PlayerInventoryComponent = InPlayerInventoryComponent;

	RefreshHotBar();
	if (UAWeekItemSlot* FirstItemSlot = Cast<UAWeekItemSlot>(HotBarGridPanel->GetChildAt(0)))
	{
		FirstItemSlot->SetHighlight(true);
	}
}

void UAWeekPlayerHotBar::RefreshHotBar()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (!PlayerInventoryComponent || !ItemSlotClass)
	{
		return;
	}
	for (int32 i = 0; i < PlayerInventoryComponent->GetHotBarInventorySize(); i++)
	{
		UAWeekItemSlot* ItemSlot = nullptr;
		if (i < HotBarGridPanel->GetChildrenCount())
		{
			ItemSlot = Cast<UAWeekItemSlot>(HotBarGridPanel->GetChildAt(i));
		}
		else
		{
			ItemSlot = CreateWidget<UAWeekItemSlot>(this, ItemSlotClass);
			HotBarGridPanel->AddChildToUniformGrid(ItemSlot, 0, i);
		}
		ItemSlot->InitializeItemSlot(PlayerInventoryComponent->GetHotBarItemAt(i).Item);
	}
}

void UAWeekPlayerHotBar::OnHotBarSelectionChanged(int32 OldIndex, int32 NewIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%s, OldIndex:%d, NewIndex:%d"),
		*FString(__FUNCTION__), OldIndex, NewIndex);
	UAWeekItemSlot* OldItemSlot = Cast<UAWeekItemSlot>(HotBarGridPanel->GetChildAt(OldIndex));
	UAWeekItemSlot* NewItemSlot = Cast<UAWeekItemSlot>(HotBarGridPanel->GetChildAt(NewIndex));
	
	if (OldItemSlot && NewItemSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("highligiht"));
		OldItemSlot->SetHighlight(false);
		NewItemSlot->SetHighlight(true);
	}
}

