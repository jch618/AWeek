// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "BuildingNotionPanel.generated.h"

/**
 * 
 */

class UCommonLazyImage;
class UCommonTextBlock;
class USizeBox;
struct FAWeekBuildingData;

UCLASS()
class AWEEK_API UBuildingNotionPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void UpdateData(const FAWeekBuildingData* Data = nullptr);
	virtual void ClearData();

	bool bTrue = true;
protected:
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* BuildingName;
	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* BuildingIcon;
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* BuildingText;
	UPROPERTY(meta=(BindWidget))
	USizeBox* SizeBox;
};
