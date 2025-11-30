// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingListViewItem.h"

#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "SettingItem.h"
#include "Components/TextBlock.h"

void USettingListViewItem::Init(USettingItem* InGameSetting)
{
	if (InGameSetting)
	{
		Setting = InGameSetting;
		DisplayText = Setting->GetDisplayName();
		Setting->OnSettingChangedEvent.AddUObject(this, &ThisClass::HandleSettingChangedApplied);
		if (SettingTextBlock)
		{
			SettingTextBlock->SetText(DisplayText);
		}
		HandleInit();
	}
}

void USettingListViewItem::NativeOnEntryReleased()
{
	Setting->OnSettingChangedEvent.RemoveAll(this);
}

FReply USettingListViewItem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem())
	{
		if (InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
		{
			if (const UWidget* FocusWidget = GetDesiredGamepadWidget())
			{
				TSharedPtr<SWidget> WidgetToFocus = FocusWidget->GetCachedWidget();
				if (WidgetToFocus.IsValid())
				{
					return FReply::Handled().SetUserFocus(WidgetToFocus.ToSharedRef(), InFocusEvent.GetCause());
				}
			}
		}
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void USettingListViewItem::HandleSettingChangedApplied(USettingItem* ChangedSetting, ESettingChangedReason Reason)
{
}
