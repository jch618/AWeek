// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekItemSlot.h"
#include "AWeek/Items/AWeekItemBase.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UAWeekItemSlot::InitializeItemSlot(const FAWeekItemData& ItemData, const int Quantity)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	
	switch (ItemData.ItemQuality)
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

	ItemIcon->SetBrushFromTexture(ItemData.AssetData.Icon);

	if (ItemData.NumericData.bIsStackable)
	{
		ItemQuantityText->SetText(FText::AsNumber(Quantity));
	}
	else
	{
		ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAWeekItemSlot::InitializeItemSlot(const TObjectPtr<UAWeekItemBase> Item)
{
	if (IsValid(Item))
	{
		InitializeItemSlot(Item->GetItemData(), Item->GetQuantity());
	}
	else
	{
		ItemQuantityText->SetVisibility(ESlateVisibility::Collapsed);
		ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		ItemBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f));
	}
}


