// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSettingTabListWidgetBase.h"

#include "AWeekSettingTabButton.h"


void UAWeekSettingTabListWidgetBase::AddTab(const FAWeekSettingTabInfo& InTabInfo)
{
	RegisteredTabs.Add(InTabInfo.Id, InTabInfo);
	
	RegisterTab(InTabInfo.Id, InTabInfo.ButtonType, nullptr);
}

void UAWeekSettingTabListWidgetBase::HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton)
{
	if (const FAWeekSettingTabInfo* TabInfo = RegisteredTabs.Find(TabNameID))
	{
		if (UAWeekSettingTabButton* SettingTabButton = Cast<UAWeekSettingTabButton>(TabButton))
		{
			SettingTabButton->SetText(TabInfo->Text);
		}
	}
}
