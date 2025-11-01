// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingRegistry.h"

#include "SettingValueItem.h"

void USettingRegistry::Init(ULocalPlayer* InLocalPlayer)
{
}

void USettingRegistry::Apply()
{
	for (auto Entry:DirtySettings)
	{
		USettingItem* Setting = Entry.Value;
		Setting->Apply();
	}

	DirtySettings.Reset();
}

void USettingRegistry::Cancel()
{
	for (auto Entry:DirtySettings)
	{
		if (USettingValueItem* Setting = Cast<USettingValueItem>(Entry.Value))
		{
			Setting->Restore();
		}
	}

	DirtySettings.Reset();
}

void USettingRegistry::RegisterSetting(USettingItem* Setting)
{
	if (Setting)
	{
		RootSettings.Add(Setting);
		InitSetting(Setting);
	}
}

void USettingRegistry::InitSetting(USettingItem* Setting)
{
	Setting->OnSettingChangedEvent.AddUObject(this, &USettingRegistry::HandleSettingApplied);
	Setting->OnSettingAppliedEvent.AddUObject(this, &USettingRegistry::HandleSettingChanged);
	
	for (USettingItem* ChildSetting: Setting->GetSettings())
	{
		InitSetting(ChildSetting);
	}
}

void USettingRegistry::HandleSettingChanged(USettingItem* Setting)
{
	DirtySettings.Add(FObjectKey(Setting), Setting);
	OnSettingChanged.Broadcast(Setting);
}

void USettingRegistry::HandleSettingApplied(USettingItem* Setting)
{
}
