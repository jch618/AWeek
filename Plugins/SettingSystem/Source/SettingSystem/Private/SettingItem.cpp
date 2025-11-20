// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItem.h"

void USettingItem::Init()
{
}

void USettingItem::AddNeighbour(USettingItem* NeighbourSetting)
{
	NeighbourSetting->OnSettingChangedEvent.AddUObject(this, &USettingItem::HandleNeighbourChanged);
}

void USettingItem::HandleNeighbourChanged(USettingItem* Setting, ESettingChangedReason Reason)
{
	if (Reason != ESettingChangedReason::NeighbourChange)
	{
		NotifySettingChanged(ESettingChangedReason::NeighbourChange);
	}
}

void USettingItem::NotifySettingChanged(const ESettingChangedReason Reason)
{
	OnSettingChangedEvent.Broadcast(this, Reason);
}

void USettingItem::Apply()
{
	OnSettingAppliedEvent.Broadcast(this);
}
