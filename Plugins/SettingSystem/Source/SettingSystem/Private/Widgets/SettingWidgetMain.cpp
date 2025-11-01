// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingWidgetMain.h"

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

void USettingWidgetMain::SetPanelSetting(FName DevName)
{
	USettingItem* Setting = Registry->FindByRootSettingDevName(DevName);
	Panel->RefreshSetting(Setting);
	// Panel에 정리
}

void USettingWidgetMain::HandleSettingChanged(USettingItem* Setting)
{
	ChangeDirtyState(true);
}
