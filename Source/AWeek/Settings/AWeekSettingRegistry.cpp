// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSettingRegistry.h"

#include "AWeekGameUserSettings.h"
#include "SettingItemCategory.h"
#include "SettingPropertyResolver.h"
#include "SettingValueScalarItem.h"
#include "AWeek/Player/AWeekLocalPlayer.h"

#define LOCTEXT_NAMESPACE "AWeek"

#define GET_GAME_SETTINGS_PATH(InLocalPlayer, Name) MakeShared<FSettingPropertyResolver>(InLocalPlayer, TArray<FString>({ \
	GET_FUNCTION_NAME_STRING_CHECKED(UAWeekLocalPlayer, GetGameUserSettings), \
	GET_FUNCTION_NAME_STRING_CHECKED(UAWeekGameUserSettings, Name)}))

void UAWeekSettingRegistry::Init(ULocalPlayer* InLocalPlayer)
{
	Super::Init(InLocalPlayer);

	OwningLocalPlayer = InLocalPlayer;
	AudioSetting = RegisterAudioSetting();
	RegisterSetting(AudioSetting);
}

USettingItem* UAWeekSettingRegistry::RegisterAudioSetting()
{
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("AudioCategory"));
	Setting->SetDisplayName(LOCTEXT("NAME_AudioCategory","Audio"));

	USettingValueScalarItem* OverallSetting = NewObject<USettingValueScalarItem>();
	OverallSetting->SetDevName(TEXT("OverallSetting"));
	OverallSetting->SetDisplayName(LOCTEXT("NAME_OverallSetting","OverallSetting"));
	OverallSetting->SetMinValue(0.0f);
	OverallSetting->SetMaxValue(2.0f);
	OverallSetting->SetInitialValue(GetDefault<UAWeekGameUserSettings>()->GetOverallVolume());
	OverallSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetOverallVolume));
	OverallSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetOverallVolume));
	Setting->AddSetting(OverallSetting);

	USettingValueScalarItem* MusicSetting = NewObject<USettingValueScalarItem>();
	MusicSetting->SetDevName(TEXT("MusicSetting"));
	MusicSetting->SetDisplayName(LOCTEXT("NAME_MusicSetting","MusicSetting"));
	MusicSetting->SetMinValue(0.0f);
	MusicSetting->SetMaxValue(2.0f);
	MusicSetting->SetInitialValue(GetDefault<UAWeekGameUserSettings>()->GetMusicVolume());
	MusicSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetMusicVolume));
	MusicSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetMusicVolume));
	Setting->AddSetting(MusicSetting);
	
	return Setting;
}
