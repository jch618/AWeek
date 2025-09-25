// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekItemSlot.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class UAWeekItemBase;
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
	void InitializeItemSlot(const FAWeekItemData& ItemData, int32 ItemQuantity);
	void InitializeItemSlot(TObjectPtr<UAWeekItemBase> ItemReference);
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantityText;
	
	void InitializeItemSlot(EAWeekItemQuality ItemQuality, const FAWeekItemNumericData& ItemNumericData,
	                        const FAWeekItemAssetData& ItemAssetData, int32 ItemQuantity);
	
};
