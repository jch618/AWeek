// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekItemSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class UAWeekItemBase;
class UAWeekInventoryToolTip;
struct FAWeekItemData;
struct FAWeekItemAssetData;
struct FAWeekItemNumericData;

enum class EAWeekItemQuality : uint8;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekItemSlot : public UAWeekActivatableWidget
{
	GENERATED_BODY()
	
public:
	void InitializeItemSlot(const FAWeekItemData& ItemData, const int Quantity);
	void InitializeItemSlot(TObjectPtr<UAWeekItemBase> Item);

	void SetHighlight(bool IsHighlighted);
protected:
	UPROPERTY(VisibleAnywhere, Category = "Item Slot", meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Item Slot", meta = (BindWidgetOptional))
	TObjectPtr<UBorder> SelectionBorder;

	UPROPERTY(EditDefaultsOnly, Category = "Item Slot")
	TSubclassOf<UAWeekInventoryToolTip> ToolTipClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Slot", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Item Slot", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantityText;

	UPROPERTY(EditAnywhere, Category = "Item Slot")
	FLinearColor DefaultBorderColor = FLinearColor::Gray;
	
	UPROPERTY(EditAnywhere, Category = "Item Slot")
	FLinearColor HighlightedBorderColor = FLinearColor::Yellow;
};
