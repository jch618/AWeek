// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingListViewItem_Scalar.h"

#include "AnalogSlider.h"
#include "SettingValueScalarItem.h"

void USettingListViewItem_Scalar::Init(USettingItem* InGameSetting)
{
	ScalarSetting = Cast<USettingValueScalarItem>(InGameSetting);
	
	Super::Init(InGameSetting);

	if (ScalarSetting)
	{
		if (Slider)
		{
			const float Value = ScalarSetting->GetValue();
			OnValueChanged(Value);
			Slider->SetValue(Value);
			Slider->SetMinValue(ScalarSetting->GetMinValue());
			Slider->SetMaxValue(ScalarSetting->GetMaxValue());
			Slider->OnValueChanged.RemoveAll(this);
			Slider->OnValueChanged.AddDynamic(this, &ThisClass::HandleSliderValueChanged);
		}
	}
}

void USettingListViewItem_Scalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USettingListViewItem_Scalar::HandleSettingChangedApplied(USettingItem* ChangedSetting,
	ESettingChangedReason Reason)
{
	Slider->SetValue(ScalarSetting->GetValue());
}

void USettingListViewItem_Scalar::HandleSliderValueChanged(float Value)
{
	ScalarSetting->SetValue(Value);
	OnValueChanged(Value);
}
