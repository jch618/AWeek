// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingWidgetMain.h"

#include "CommonTabListWidgetBase.h"
#include "SettingRegistry.h"
#include "Widgets/SettingWidgetPanel.h"

void USettingWidgetMain::NativeConstruct()
{
	Super::NativeConstruct();
	Registry = CreateRegistry();
	Registry->OnSettingChanged.AddUObject(this, &ThisClass::HandleSettingChanged);
}

void USettingWidgetMain::Apply()
{
	if (Registry)
	{
		Registry->Apply();
	}
	ChangeDirtyState(false);
}

void USettingWidgetMain::Cancel()
{
	if (Registry)
	{
		Registry->Cancel();
	}
	ChangeDirtyState(false);
}

void USettingWidgetMain::RegisterSettingTab(const FTabRegistry& TabRegistry)
{
	TabList->RegisterTab(TabRegistry.DevName, TabRegistry.ButtonWidgetType, nullptr);
}

void USettingWidgetMain::SetPanelSetting(FName DevName)
{
	USettingItem* Setting = Registry->FindByRootSettingDevName(DevName);
	Panel->RefreshSetting(Setting);
}

void USettingWidgetMain::HandleSettingChanged(USettingItem* Setting)
{
	ChangeDirtyState(true);
}
