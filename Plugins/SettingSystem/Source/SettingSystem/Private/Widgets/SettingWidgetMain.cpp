// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingWidgetMain.h"

#include "CommonTabListWidgetBase.h"
#include "SettingRegistry.h"
#include "Widgets/SettingWidgetPanel.h"

void USettingWidgetMain::NativeConstruct()
{
	Registry = CreateRegistry();
	Registry->Init(GetOwningLocalPlayer());
	Registry->OnSettingChanged.AddUObject(this, &ThisClass::HandleSettingChanged);
	Super::NativeConstruct();
	ChangeDirtyState(false);
}

UWidget* USettingWidgetMain::NativeGetDesiredFocusTarget() const
{
	return Panel;
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
}

USettingItem* USettingWidgetMain::FindByRootSettingDevName(const FName& DevName)
{
	if (Registry)
	{
		return Registry->FindByRootSettingDevName(DevName);
	}

	return nullptr;
}

void USettingWidgetMain::HandleSettingChanged(USettingItem* Setting, ESettingChangedReason Reason)
{
	if (Reason == ESettingChangedReason::Change)
	{
		ChangeDirtyState(true);
	}
}
