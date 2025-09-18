#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekInventoryToolTip.generated.h"

class UTextBlock;

class UAWeekInventoryItemSlot;

UCLASS()
class AWEEK_API UAWeekInventoryToolTip : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	void InitializeToolTip(TObjectPtr<UAWeekInventoryItemSlot> NewItemSlotWidget);

protected:
	TObjectPtr<UAWeekInventoryItemSlot> ItemSlotWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UsageText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ArmorRating;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxStackSize;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StackWeight;
};
