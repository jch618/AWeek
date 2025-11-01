// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingWidgetMain.h"

#include "SettingRegistry.h"

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

void USettingWidgetMain::HandleSettingChanged(USettingItem* Setting)
{
	ChangeDirtyState(true);
}
