// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingListViewItem.h"

#include "SettingItem.h"

void USettingListViewItem::Init(USettingItem* InGameSetting)
{
	if (InGameSetting)
	{
		Setting = InGameSetting;
		DisplayText = Setting->GetDisplayName();
		Setting->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleSettingChangedApplied);
		HandleInit();
	}
}

void USettingListViewItem::HandleSettingChangedApplied(USettingItem* ChangedSetting)
{
}
