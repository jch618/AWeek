// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingValueItem_VideoQuality.h"

#include "AWeek/Settings/AWeekGameUserSettings.h"

#define LOCTEXT_NAMESPACE "AWeekSettings"

void USettingValueItem_VideoQuality::Init()
{
	Super::Init();
	
	AddNumOption(0, LOCTEXT("OverallScalabilityLevel_Low", "Low"));
	AddNumOption(1, LOCTEXT("OverallScalabilityLevel_Medium", "Medium"));
	AddNumOption(2, LOCTEXT("OverallScalabilityLevel_High", "High"));
	AddNumOption(3, LOCTEXT("OverallScalabilityLevel_Epic", "Epic"));
	AddNumOption(4, LOCTEXT("OverallScalabilityLevel_Cinematic", "Cinematic"));
	AddNumOption(5, LOCTEXT("OverallScalabilityLevel_Custom", "Custom"));
}

void USettingValueItem_VideoQuality::Store()
{
}

void USettingValueItem_VideoQuality::Restore()
{
}

void USettingValueItem_VideoQuality::Reset()
{
}

int32 USettingValueItem_VideoQuality::GetValueOptionIndex() const
{
	if (UAWeekGameUserSettings* UserSettings = UAWeekGameUserSettings::Get())
	{
		const int32 Level = UserSettings->GetOverallScalabilityLevel();;
		if (Level != INDEX_NONE)
		{
			return Level;
		}
	}
	
	return OptionValue.Num() - 1;
}

void USettingValueItem_VideoQuality::SetValueOptionByIndex(int32 Index)
{
	if (Index != OptionValue.Num() - 1)
	{
		if (UAWeekGameUserSettings* UserSettings = UAWeekGameUserSettings::Get())
		{
			UserSettings->SetOverallScalabilityLevel(Index);
		}
	}
	
	NotifySettingChanged(ESettingChangedReason::Change);
}
