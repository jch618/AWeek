// game
#include "AWeek/UI/Inventory/AWeekInventoryTooltip.h"
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Data/AWeekItemDataStructs.h"

// engine
#include "Components/TextBlock.h"

void UAWeekInventoryToolTip::InitializeToolTip(const TObjectPtr<UAWeekItemBase> Item)
{
	if (!IsValid(Item))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Item is invalid!"), *FString(__FUNCTION__));
		return;
	}
	InitializeToolTip(Item->GetItemData(), Item->GetQuantity());
}

void UAWeekInventoryToolTip::InitializeToolTip(const FAWeekItemData& ItemData, const int ItemQuantity)
{
	switch (ItemData.ItemType)
	{
	case EAWeekItemType::Weapon:
		break;

	case EAWeekItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		//SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
		
	case EAWeekItemType::Ammo:
		ItemType->SetText(FText::FromString("Ammo"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		//SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
		
	case EAWeekItemType::Generic:
		ItemType->SetText(FText::FromString("Generic Item"));
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		//SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;

	default:
		break;
	}

	ItemName->SetText(ItemData.TextData.Name);
	DamageValue->SetVisibility(ESlateVisibility::Collapsed);
	// DamageValue->SetText(FText::AsNumber(ItemData.WeaponData.Damage));
	UsageText->SetText(ItemData.TextData.UsageText);
	ItemDescription->SetText(ItemData.TextData.Description);
	//SellValue->SetText(FText::AsNumber(ItemData.ItemStatistics.SellValue));

	const FString WeightInfo = "Weight: " + FString::SanitizeFloat(ItemData.NumericData.Weight);
	StackWeight->SetText(FText::FromString(WeightInfo));
	
	if (ItemData.NumericData.bIsStackable)
	{
		const FString StackInfo = "Max stack size: " + FString::FromInt(ItemData.NumericData.MaxStackSize);
		MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
