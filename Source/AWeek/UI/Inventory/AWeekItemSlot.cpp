// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekItemSlot.h"
#include "AWeek/Items/AWeekItemBase.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UAWeekItemSlot::InitializeItemSlot(const FAWeekItemData& ItemData, int32 ItemQuantity)
{
	InitializeItemSlot(ItemData.ItemQuality, ItemData.NumericData, ItemData.AssetData, ItemQuantity);
}

void UAWeekItemSlot::InitializeItemSlot(TObjectPtr<UAWeekItemBase> ItemReference)
{
	if (IsValid(ItemReference))
	{
		InitializeItemSlot(ItemReference->ItemQuality, ItemReference->NumericData, ItemReference->AssetData, ItemReference->Quantity);
	}
	else
	{
		ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		ItemBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f));
	}
}

void UAWeekItemSlot::InitializeItemSlot(EAWeekItemQuality ItemQuality, const FAWeekItemNumericData& ItemNumericData,
                                        const FAWeekItemAssetData& ItemAssetData, int32 ItemQuantity)
{
	switch (ItemQuality)
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

	ItemIcon->SetBrushFromTexture(ItemAssetData.Icon);

	if (ItemNumericData.bIsStackable)
	{
		ItemQuantityText->SetText(FText::AsNumber(ItemQuantity));
	}
	else
	{
		ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
