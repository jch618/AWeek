// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SettingListViewItem.h"
#include "SettingListViewItem_Scalar.generated.h"

class UAnalogSlider;
class USettingValueScalarItem;
/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingListViewItem_Scalar : public USettingListViewItem
{
	GENERATED_BODY()
public:
	virtual void Init(USettingItem* InGameSetting) override;

protected:
	virtual void NativeOnInitialized() override;

	virtual void HandleSettingChangedApplied(USettingItem* ChangedSetting) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnValueChanged(float Value);
	
	UFUNCTION()
	void HandleSliderValueChanged(float Value);
	
	UPROPERTY()
	TObjectPtr<USettingValueScalarItem> ScalarSetting;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UAnalogSlider> Slider;
};
