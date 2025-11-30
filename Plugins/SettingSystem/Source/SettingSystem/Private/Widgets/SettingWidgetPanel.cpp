// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingWidgetPanel.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "SettingItem.h"
#include "Widgets/SettingListView.h"

void USettingWidgetPanel::RefreshSetting(USettingItem* RootSetting)
{
	TArray<UObject*> CurrentSettings;
	if (RootSetting)
	{
		for (USettingItem* Child : RootSetting->GetSettings())
		{
			AppendSettingChild(Child, CurrentSettings);
		}
	}
	ListView->SetListItems(CurrentSettings);
}

FReply USettingWidgetPanel::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem())
	{
		if (InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
		{
			if (ListView->GetNumItems() > 0)
			{
				ListView->NavigateToIndex(0);
				ListView->SetSelectedIndex(0);

				return FReply::Handled()
					.SetUserFocus(ListView->TakeWidget(), EFocusCause::SetDirectly);
			}
		}
	}

	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void USettingWidgetPanel::AppendSettingChild(USettingItem* Setting, TArray<UObject*>& InOutSettings)
{
	InOutSettings.Add(Setting);

	for (USettingItem* Child : Setting->GetSettings())
	{
		AppendSettingChild(Child, InOutSettings);
	}
}
