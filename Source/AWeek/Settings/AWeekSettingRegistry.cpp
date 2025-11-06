// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSettingRegistry.h"

#include "AWeekGameUserSettings.h"
#include "SettingItemCategory.h"
#include "SettingPropertyResolver.h"
#include "SettingValueDiscreteItem_Bool.h"
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
	GameplaySetting = RegisterGameplaySetting();
	RegisterSetting(GameplaySetting);
	
	AudioSetting = RegisterAudioSetting();
	RegisterSetting(AudioSetting);
}

void UAWeekSettingRegistry::Apply()
{
	Super::Apply();
	if (UAWeekLocalPlayer* LocalPlayer = Cast<UAWeekLocalPlayer>(OwningLocalPlayer))
	{
		LocalPlayer->GetGameUserSettings()->ApplySettings(false);
	}
}

void UAWeekSettingRegistry::Cancel()
{
	Super::Cancel();
}

USettingItem* UAWeekSettingRegistry::RegisterGameplaySetting()
{
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("GameplayCategory"));
	Setting->SetDisplayName(LOCTEXT("NAME_GameplayCategory","Gameplay"));

	USettingValueScalarItem* MouseSensitivityX = NewObject<USettingValueScalarItem>();
	MouseSensitivityX->SetDevName(TEXT("MouseSensitivityX"));
	MouseSensitivityX->SetDisplayName(LOCTEXT("NAME_MouseSensitivityX","MouseSensitivityX"));
	MouseSensitivityX->SetMinValue(0.0f);
	MouseSensitivityX->SetMaxValue(2.0f);
	MouseSensitivityX->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetMouseSensitivityX());
	MouseSensitivityX->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetMouseSensitivityX));
	MouseSensitivityX->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetMouseSensitivityX));
	Setting->AddSetting(MouseSensitivityX);

	USettingValueScalarItem* MouseSensitivityY = NewObject<USettingValueScalarItem>();
	MouseSensitivityY->SetDevName(TEXT("MouseSensitivityY"));
	MouseSensitivityY->SetDisplayName(LOCTEXT("NAME_MouseSensitivityY","MouseSensitivityY"));
	MouseSensitivityY->SetMinValue(0.0f);
	MouseSensitivityY->SetMaxValue(2.0f);
	MouseSensitivityY->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetMouseSensitivityY());
	MouseSensitivityY->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetMouseSensitivityY));
	MouseSensitivityY->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetMouseSensitivityY));
	Setting->AddSetting(MouseSensitivityY);

	USettingValueDiscreteItem_Bool* FullScreen = NewObject<USettingValueDiscreteItem_Bool>();
	FullScreen->SetDevName(TEXT("FullScreenMode"));
	FullScreen->SetDisplayName(LOCTEXT("FullScreenMode","IsFullScreen"));
	FullScreen->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetFullScreenModeTemp());
	FullScreen->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetFullScreenModeTemp));
	FullScreen->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetFullScreenModeTemp));
	Setting->AddSetting(FullScreen);
	
	return Setting;
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
	OverallSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetOverallVolume());
	OverallSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetOverallVolume));
	OverallSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetOverallVolume));
	Setting->AddSetting(OverallSetting);

	USettingValueScalarItem* MusicSetting = NewObject<USettingValueScalarItem>();
	MusicSetting->SetDevName(TEXT("MusicSetting"));
	MusicSetting->SetDisplayName(LOCTEXT("NAME_MusicSetting","MusicSetting"));
	MusicSetting->SetMinValue(0.0f);
	MusicSetting->SetMaxValue(2.0f);
	MusicSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetMusicVolume());
	MusicSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetMusicVolume));
	MusicSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetMusicVolume));
	Setting->AddSetting(MusicSetting);
	
	return Setting;
}
