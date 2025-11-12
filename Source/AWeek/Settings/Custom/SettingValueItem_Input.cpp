// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingValueItem_Input.h"

#include "Internationalization/TextNamespaceUtil.h"

void USettingValueItem_Input::Init(const UEnhancedPlayerMappableKeyProfile* KeyProfile,
                                   const FKeyMappingRow& MappingData, const FPlayerMappableKeyQueryOptions& InQueryOptions,
                                   UEnhancedInputUserSettings* NewInputSetting)
{
	ProfileIdentifier = KeyProfile->GetProfileIdentifer();
	QueryOptions = InQueryOptions;
	InputSetting = NewInputSetting;

	const FString InitialDevName = TEXT("Input_") + ActionMappingName.ToString();
	SetDevName(*InitialDevName);
	
	for (const FPlayerKeyMapping& Mapping : MappingData.Mappings)
	{
		if (!KeyProfile->DoesMappingPassQueryOptions(Mapping, QueryOptions))
		{
			continue;
		}

		ActionMappingName = Mapping.GetMappingName();
		InitialKey = Mapping.GetCurrentKey();
		InitialSlot = Mapping.GetSlot();
		const FText& InitialDisplayName = Mapping.GetDisplayName();
		
		if (!InitialDisplayName.IsEmpty())
		{
			SetDisplayName(InitialDisplayName);
			break;
		}
	}
}

void USettingValueItem_Input::Reset()
{
	Super::Reset();
	FMapPlayerKeyArgs Args = {};
	Args.MappingName = ActionMappingName;

	FGameplayTagContainer FailReason;
	InputSetting->ResetAllPlayerKeysInRow(Args, FailReason);
	NotifySettingChanged();
}

void USettingValueItem_Input::Store()
{
	if (const UEnhancedPlayerMappableKeyProfile* Profile = InputSetting->GetKeyProfileWithIdentifier(ProfileIdentifier))
	{
		if(const FKeyMappingRow* Row = Profile->FindKeyMappingRow(ActionMappingName))
		{
			for (const FPlayerKeyMapping& Mapping : Row->Mappings)
			{
				if (Profile->DoesMappingPassQueryOptions(Mapping, QueryOptions))
				{
					if (ActionMappingName == Mapping.GetMappingName())
					{
						InitialKey = Mapping.GetCurrentKey();
						break;
					}
				}
			}
		}
	}
}

void USettingValueItem_Input::Restore()
{
	ChangeKey(static_cast<int32>(InitialSlot), InitialKey);
}

bool USettingValueItem_Input::ChangeKey(int32 InSlot, FKey InKey)
{
	if (!InKey.IsGamepadKey())
	{
		FMapPlayerKeyArgs Args = {};
		Args.MappingName = ActionMappingName;
		Args.Slot = static_cast<EPlayerMappableKeySlot>(static_cast<uint8>(InSlot));
		Args.NewKey = InKey;

		
		FGameplayTagContainer FailureReason;
		InputSetting->MapPlayerKey(Args, FailureReason);
		NotifySettingChanged();
	}

	return false;
}
