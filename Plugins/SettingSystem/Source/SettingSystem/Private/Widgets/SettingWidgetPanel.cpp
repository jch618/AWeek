// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingWidgetPanel.h"

#include "SettingItem.h"
#include "Widgets/SettingListView.h"

void USettingWidgetPanel::RefreshSetting(USettingItem* RootSetting)
{
	TArray<USettingItem*> CurrentSettings;
	if (RootSetting)
	{
		AppendSettingChild(RootSetting, CurrentSettings);
	}

	ListView->SetListItems(CurrentSettings);
}

void USettingWidgetPanel::AppendSettingChild(USettingItem* Setting, TArray<USettingItem*>& InOutSettings)
{
	InOutSettings.Add(Setting);

	for (USettingItem* Child: Setting->GetSettings())
	{
		InOutSettings.Add(Child);
		AppendSettingChild(Child, InOutSettings);
	}
}
