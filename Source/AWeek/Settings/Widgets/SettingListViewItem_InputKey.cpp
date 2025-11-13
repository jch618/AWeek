// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingListViewItem_InputKey.h"

#include "AWeek/Settings/Custom/SettingValueItem_Input.h"
#include "Components/InputKeySelector.h"

void USettingListViewItem_InputKey::Init(USettingItem* InGameSetting)
{
	Super::Init(InGameSetting);
	InputSetting = Cast<USettingValueItem_Input>(InGameSetting);
	if (InputSetting)
	{
		FInputChord InputChord;
		InputChord.Key = InputSetting->GetKey();
		KeySelector->SetSelectedKey(InputChord);
		
		OnKeySelected(InputSetting->GetKey());
		if (ensure(KeySelector))
		{
			KeySelector->OnKeySelected.AddDynamic(this, &ThisClass::HandleKeySelected);
		}
	}
}

void USettingListViewItem_InputKey::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USettingListViewItem_InputKey::HandleKeySelected(FInputChord SelectedKey)
{
	FKey Key = SelectedKey.Key;
	if (Key.IsValid())
	{
		InputSetting->ChangeKey(0, Key);
		OnKeySelected(Key);
	}
}
