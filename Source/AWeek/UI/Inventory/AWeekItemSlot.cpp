// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekItemSlot.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/UI/Inventory/AWeekInventoryToolTip.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UAWeekItemSlot::InitializeItemSlot(const FAWeekItemData& ItemData, const int Quantity)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	
	if (ToolTipClass)
	{
		UAWeekInventoryToolTip* ToolTip = CreateWidget<UAWeekInventoryToolTip>(this, ToolTipClass);
		if (IsValid(ToolTip))
		{
			ToolTip->InitializeToolTip(ItemData, Quantity);
			SetToolTip(ToolTip);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: ToolTip is invalid"), *FString(__FUNCTION__));
		}
	}
	ItemBorder->SetBrushColor(UAWeekItemQualityHelper::GetQualityColor(ItemData.ItemQuality));

	//
	// UE_LOG(LogTemp, Warning, TEXT("Update Item Icon"));
	ItemIcon->SetVisibility(ESlateVisibility::Visible);
	ItemIcon->SetBrushFromTexture(ItemData.AssetData.Icon);

	if (ItemData.NumericData.bIsStackable)
	{
		ItemQuantityText->SetVisibility(ESlateVisibility::Visible);
		ItemQuantityText->SetText(FText::AsNumber(Quantity));
	}
	else
	{
		ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAWeekItemSlot::InitializeItemSlot(const TObjectPtr<UAWeekItemBase> Item)
{
	if (SelectionBorder)
	{
		SelectionBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (IsValid(Item))
	{
		InitializeItemSlot(Item->GetItemData(), Item->GetQuantity());
	}
	else
	{
		ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		ItemBorder->SetBrushColor(DefaultBorderColor);
	}
}

void UAWeekItemSlot::SetHighlight(bool IsHighlighted)
{
	if (SelectionBorder)
	{
		if (IsHighlighted)
		{
			SelectionBorder->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SelectionBorder->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


