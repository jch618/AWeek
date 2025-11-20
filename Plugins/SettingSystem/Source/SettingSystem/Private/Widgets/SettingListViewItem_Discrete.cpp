// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingListViewItem_Discrete.h"

#include "CommonRotator.h"
#include "SettingValueDiscreteItem.h"

void USettingListViewItem_Discrete::Init(USettingItem* InGameSetting)
{
	DiscreteSetting = Cast<USettingValueDiscreteItem>(InGameSetting);
	
	Super::Init(InGameSetting);

	if (DiscreteSetting)
	{
		// Widget 값 초기 세팅
		const TArray<FText> OptionTexts = DiscreteSetting->GetValueOptions();
		if (OptionTexts.Num() > 0)
		{
			Rotator->PopulateTextLabels(OptionTexts);
			Rotator->SetSelectedItem(DiscreteSetting->GetValueOptionIndex());
		}
		OnValueChanged(DiscreteSetting->GetValueOptionIndex());
		Rotator->OnRotatedEvent.RemoveAll(this);
		NextButton->OnClicked().RemoveAll(this);
		PrevButton->OnClicked().RemoveAll(this);
		
		Rotator->OnRotatedEvent.AddUObject(this, &ThisClass::HandleRotatorValueChanged);
		NextButton->OnClicked().AddUObject(this, &ThisClass::HandleNextButtonClicked);
		PrevButton->OnClicked().AddUObject(this, &ThisClass::HandlePrevButtonClicked);
	}
}

void USettingListViewItem_Discrete::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USettingListViewItem_Discrete::HandleSettingChangedApplied(USettingItem* ChangedSetting, ESettingChangedReason Reason)
{
	Rotator->SetSelectedItem(DiscreteSetting->GetValueOptionIndex());
}

void USettingListViewItem_Discrete::HandleRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	DiscreteSetting->SetValueOptionByIndex(Value);
	OnValueChanged(Value);
}

void USettingListViewItem_Discrete::HandleNextButtonClicked()
{
	Rotator->ShiftTextRight();
	DiscreteSetting->SetValueOptionByIndex(Rotator->GetSelectedIndex());
}

void USettingListViewItem_Discrete::HandlePrevButtonClicked()
{
	Rotator->ShiftTextLeft();
	DiscreteSetting->SetValueOptionByIndex(Rotator->GetSelectedIndex());
}
