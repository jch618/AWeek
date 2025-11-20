// game
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"

#include "AWeek/Items/AWeekItemBase.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAWeekHeldItemVisual::InitializeHeldItemVisual(const TObjectPtr<UAWeekItemBase> InItem) const
{
	ItemIcon->SetBrushFromTexture(InItem->GetAssetData().Icon);

	InItem->GetNumericData().bIsStackable
		? ItemQuantity->SetText(FText::AsNumber(InItem->GetQuantity()))
		: ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

	ItemBorder->SetBrushColor(UAWeekItemQualityHelper::GetQualityColor(InItem->GetItemQuality()));
	
	ItemQuantity->SetText(FText::AsNumber(InItem->GetQuantity()));
}
