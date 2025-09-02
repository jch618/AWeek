#include "Components/TextBlock.h"

#include "AWeek/UI/Inventory/AWeekInventoryTooltip.h"
#include "AWeek/UI/Inventory/AWeekInventoryItemSlot.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"

void UAWeekInventoryToolTip::InitializeToolTip(TObjectPtr<UAWeekInventoryItemSlot> NewItemSlotWidget)
{
	const UAWeekItemBase* ItemBeingHovered = NewItemSlotWidget->GetItemReference();
	if (!IsValid(ItemBeingHovered))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryTooTip: ItemBeingHovered is null!"));
		return;
	}

	switch (ItemBeingHovered->ItemType)
	{
	case EAWeekItemType::Armor:
		break;

	case EAWeekItemType::Weapon:
		break;

	case EAWeekItemType::Shield:
		break;

	case EAWeekItemType::Spell:
		break;

	case EAWeekItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		//SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;

	case EAWeekItemType::Quest:
		break;

	case EAWeekItemType::Mundane:
		ItemType->SetText(FText::FromString("Mundane Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		//SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;

	default:
		break;
	}

	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorRating));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	//SellValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.SellValue));

	const FString WeightInfo = "Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight());
	StackWeight->SetText(FText::FromString(WeightInfo));

	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		const FString StackInfo = "Max stack size: " + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize);
		MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
