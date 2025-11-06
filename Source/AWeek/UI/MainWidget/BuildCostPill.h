// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "BuildCostPill.generated.h"

/**
 * 
 */
class UCommonLazyImage;
class UCommonTextBlock;
class UAWeekInventoryComponent;

UCLASS()
class AWEEK_API UBuildCostPill : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	bool CheckItem();
	bool RemoveItem();

	void SetName(FText Name);
	void SetImage(UTexture2D* Image);
	void SetCount(int32 Count);
	void SetItemID(FName ID);

	UPROPERTY()
	UAWeekInventoryComponent* InventoryComponent;

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* ItemImage;
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ItemName;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ItemCount;

	UPROPERTY() FName ItemID;
	UPROPERTY() int32 RequiredCount = 1;
};
