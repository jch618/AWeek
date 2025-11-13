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

	// switch (ItemData.ItemQuality)
	// {
	// case EAWeekItemQuality::Poor:
	// 	ItemBorder->SetBrushColor(FLinearColor::Gray);
	// 	break;
	// case EAWeekItemQuality::Common:
	// 	ItemBorder->SetBrushColor(FLinearColor::White);
	// 	break;
	// case EAWeekItemQuality::Rare:
	// 	ItemBorder->SetBrushColor(FLinearColor(0.0f, 0.51f, 0.169f));
	// 	break;
	// case EAWeekItemQuality::Epic:
	// 	ItemBorder->SetBrushColor(FLinearColor(0.0f, 0.4f, 0.75f));
	// 	break;
	// case EAWeekItemQuality::Grandmaster:
	// 	ItemBorder->SetBrushColor(FLinearColor(1.0f, 0.45f, 0.0f)); // orange
	// 	break;
	// default:
	// 	break;
	// }

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
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
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
	if (IsHighlighted)
	{
		ItemBorder->SetBrushColor(HighlightedBorderColor);
	}
	else
	{
		ItemBorder->SetBrushColor(DefaultBorderColor);
	}
}


