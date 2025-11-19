// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSettingRegistry.h"

#include "AWeekGameUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "SettingItemCategory.h"
#include "SettingPropertyResolver.h"
#include "SettingValueDiscreteItem_Bool.h"
#include "SettingValueDiscreteItem_Enum.h"
#include "SettingValueDiscreteItem_Num.h"
#include "SettingValueScalarItem.h"
#include "AWeek/Player/AWeekLocalPlayer.h"
#include "Custom/SettingValueItem_Input.h"
#include "Custom/SettingValueItem_VideoQuality.h"
#include "UserSettings/EnhancedInputUserSettings.h"

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
	
	GraphicsSetting = RegisterGraphicSetting();
	RegisterSetting(GraphicsSetting);
	
	AudioSetting = RegisterAudioSetting();
	RegisterSetting(AudioSetting);

	KeyboardAndMouseSetting = RegisterKeyboardAndMouseSetting();
	RegisterSetting(KeyboardAndMouseSetting);
}

void UAWeekSettingRegistry::Apply()
{
	Super::Apply();
	if (UAWeekLocalPlayer* LocalPlayer = Cast<UAWeekLocalPlayer>(OwningLocalPlayer))
	{
		LocalPlayer->GetGameUserSettings()->ApplySettings(false);
		const UEnhancedInputLocalPlayerSubsystem* InputSubsystem = OwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		UEnhancedInputUserSettings* UserSettings = InputSubsystem->GetUserSettings();
		UserSettings->ApplySettings();
		UserSettings->SaveSettings();
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
	
	return Setting;
}

USettingItem* UAWeekSettingRegistry::RegisterGraphicSetting()
{
	const UAWeekGameUserSettings* UserSettings = UAWeekGameUserSettings::Get();
	
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("GraphicsCategory"));
	Setting->SetDisplayName(LOCTEXT("SETTING_GraphicsCategory","Graphics"));
	
	USettingItemCategory* Display = NewObject<USettingItemCategory>();
	Display->SetDevName(TEXT("Display"));
	Display->SetDisplayName(LOCTEXT("SETTING_Display","Display"));
	Setting->AddSetting(Display);
	{
		USettingValueDiscreteItem_Enum* WindowMode = NewObject<USettingValueDiscreteItem_Enum>();
		WindowMode->SetDevName(TEXT("WindowMode"));
		WindowMode->SetDisplayName(LOCTEXT("SETTING_WindowMode","WindowMode"));
		WindowMode->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetFullscreenMode));
		WindowMode->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetFullscreenMode));
		WindowMode->SetDefaultValueEnum(EWindowMode::Fullscreen);
		WindowMode->AddEnumOption(EWindowMode::Fullscreen, LOCTEXT("SETTING_WindowModeFullScreen", "FullScreen"));
		WindowMode->AddEnumOption(EWindowMode::Windowed, LOCTEXT("SETTING_WindowModeWindowed", "Windowed"));
		WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, LOCTEXT("SETTING_WindowModeWindowedFullScreen", "WindowedFullScreen"));
		Display->AddSetting(WindowMode);
	}
	

	USettingItemCategory* Graphics = NewObject<USettingItemCategory>();
	Graphics->SetDevName(TEXT("Graphics"));
	Graphics->SetDisplayName(LOCTEXT("SETTING_Graphics","Graphics"));
	Setting->AddSetting(Graphics);
	{
		USettingValueScalarItem* GammaSetting = NewObject<USettingValueScalarItem>();
		GammaSetting->SetDevName(TEXT("Gamma"));
		GammaSetting->SetDisplayName(LOCTEXT("SETTING_Gamma","Gamma"));
		GammaSetting->SetMinValue(1.0f);
		GammaSetting->SetMaxValue(5.0f);
		GammaSetting->SetDefaultValue(2.2f);
		GammaSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetGamma));
		GammaSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetGamma));
		Graphics->AddSetting(GammaSetting);
	}
	{
		USettingValueItem_VideoQuality* OverallQuality = NewObject<USettingValueItem_VideoQuality>();
		OverallQuality->SetDevName(TEXT("OverallQuality"));
		OverallQuality->SetDisplayName(LOCTEXT("SETTING_OverallQuality","OverallQuality"));
		Graphics->AddSetting(OverallQuality);

		USettingValueScalarItem* ResolutionScale = NewObject<USettingValueScalarItem>();
		ResolutionScale->SetDevName(TEXT("ResolutionScale"));
		ResolutionScale->SetDisplayName(LOCTEXT("SETTING_ResolutionScale","ResolutionScale"));
		ResolutionScale->SetMinValue(0.0f);
		ResolutionScale->SetMaxValue(1.0f);
		ResolutionScale->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetResolutionScaleNormalized());
		ResolutionScale->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetResolutionScaleNormalized));
		ResolutionScale->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetResolutionScaleNormalized));
		ResolutionScale->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(ResolutionScale);
		Graphics->AddSetting(ResolutionScale);

		USettingValueDiscreteItem_Num* ViewDistance = NewObject<USettingValueDiscreteItem_Num>();
		ViewDistance->SetDevName(TEXT("ViewDistanceSetting"));
		ViewDistance->SetDisplayName(LOCTEXT("SETTING_ViewDistance","ViewDistance"));
		ViewDistance->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetViewDistanceQuality));
		ViewDistance->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetViewDistanceQuality));
		ViewDistance->AddNumOption(0, LOCTEXT("ViewDistance_Low", "Low"));
		ViewDistance->AddNumOption(1, LOCTEXT("ViewDistance_Medium", "Medium"));
		ViewDistance->AddNumOption(2, LOCTEXT("ViewDistance_High", "High"));
		ViewDistance->AddNumOption(3, LOCTEXT("ViewDistance_Epic", "Epic"));
		ViewDistance->AddNumOption(4, LOCTEXT("ViewDistancew_Cinematic", "Cinematic"));
		ViewDistance->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(ViewDistance);
		Graphics->AddSetting(ViewDistance);

		USettingValueDiscreteItem_Num* Shadow = NewObject<USettingValueDiscreteItem_Num>();
		Shadow->SetDevName(TEXT("ShadowSetting"));
		Shadow->SetDisplayName(LOCTEXT("SETTING_Shadow","Shadow"));
		Shadow->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetShadowQuality));
		Shadow->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetShadowQuality));
		Shadow->AddNumOption(0, LOCTEXT("Shadow_Low", "Low"));
		Shadow->AddNumOption(1, LOCTEXT("Shadow_Medium", "Medium"));
		Shadow->AddNumOption(2, LOCTEXT("Shadow_High", "High"));
		Shadow->AddNumOption(3, LOCTEXT("Shadow_Epic", "Epic"));
		Shadow->AddNumOption(4, LOCTEXT("Shadow_Cinematic", "Cinematic"));
		Shadow->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(Shadow);
		Graphics->AddSetting(Shadow);

		USettingValueDiscreteItem_Num* GlobalIllumination = NewObject<USettingValueDiscreteItem_Num>();
		GlobalIllumination->SetDevName(TEXT("GlobalIlluminationSetting"));
		GlobalIllumination->SetDisplayName(LOCTEXT("SETTING_GlobalIllumination","GlobalIllumination"));
		GlobalIllumination->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetGlobalIlluminationQuality));
		GlobalIllumination->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetGlobalIlluminationQuality));
		GlobalIllumination->AddNumOption(0, LOCTEXT("GlobalIllumination_Low", "Low"));
		GlobalIllumination->AddNumOption(1, LOCTEXT("GlobalIllumination_Medium", "Medium"));
		GlobalIllumination->AddNumOption(2, LOCTEXT("GlobalIllumination_High", "High"));
		GlobalIllumination->AddNumOption(3, LOCTEXT("GlobalIllumination_Epic", "Epic"));
		GlobalIllumination->AddNumOption(4, LOCTEXT("GlobalIllumination_Cinematic", "Cinematic"));
		GlobalIllumination->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(GlobalIllumination);
		Graphics->AddSetting(GlobalIllumination);

		USettingValueDiscreteItem_Num* Reflection = NewObject<USettingValueDiscreteItem_Num>();
		Reflection->SetDevName(TEXT("ReflectionSetting"));
		Reflection->SetDisplayName(LOCTEXT("SETTING_Reflection","Reflection"));
		Reflection->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetReflectionQuality));
		Reflection->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetReflectionQuality));
		Reflection->AddNumOption(0, LOCTEXT("Reflection_Low", "Low"));
		Reflection->AddNumOption(1, LOCTEXT("Reflection_Medium", "Medium"));
		Reflection->AddNumOption(2, LOCTEXT("Reflection_High", "High"));
		Reflection->AddNumOption(3, LOCTEXT("Reflection_Epic", "Epic"));
		Reflection->AddNumOption(4, LOCTEXT("Reflection_Cinematic", "Cinematic"));
		Reflection->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(Reflection);
		Graphics->AddSetting(Reflection);

		USettingValueDiscreteItem_Num* AntiAliasing = NewObject<USettingValueDiscreteItem_Num>();
		AntiAliasing->SetDevName(TEXT("AntiAliasingSetting"));
		AntiAliasing->SetDisplayName(LOCTEXT("SETTING_AntiAliasing","AntiAliasing"));
		AntiAliasing->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetAntiAliasingQuality));
		AntiAliasing->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetAntiAliasingQuality));
		AntiAliasing->AddNumOption(0, LOCTEXT("AntiAliasing_Low", "Low"));
		AntiAliasing->AddNumOption(1, LOCTEXT("AntiAliasing_Medium", "Medium"));
		AntiAliasing->AddNumOption(2, LOCTEXT("AntiAliasing_High", "High"));
		AntiAliasing->AddNumOption(3, LOCTEXT("AntiAliasing_Epic", "Epic"));
		AntiAliasing->AddNumOption(4, LOCTEXT("AntiAliasing_Cinematic", "Cinematic"));
		AntiAliasing->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(AntiAliasing);
		Graphics->AddSetting(AntiAliasing);

		USettingValueDiscreteItem_Num* Textures = NewObject<USettingValueDiscreteItem_Num>();
		Textures->SetDevName(TEXT("TexturesSetting"));
		Textures->SetDisplayName(LOCTEXT("SETTING_Textures","Textures"));
		Textures->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetTextureQuality));
		Textures->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetTextureQuality));
		Textures->AddNumOption(0, LOCTEXT("Textures_Low", "Low"));
		Textures->AddNumOption(1, LOCTEXT("Textures_Medium", "Medium"));
		Textures->AddNumOption(2, LOCTEXT("Textures_High", "High"));
		Textures->AddNumOption(3, LOCTEXT("Textures_Epic", "Epic"));
		Textures->AddNumOption(4, LOCTEXT("Textures_Cinematic", "Cinematic"));
		Textures->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(Textures);
		Graphics->AddSetting(Textures);

		USettingValueDiscreteItem_Num* VisualEffects = NewObject<USettingValueDiscreteItem_Num>();
		VisualEffects->SetDevName(TEXT("VisualEffectsSetting"));
		VisualEffects->SetDisplayName(LOCTEXT("SETTING_VisualEffects","VisualEffects"));
		VisualEffects->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetVisualEffectQuality));
		VisualEffects->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetVisualEffectQuality));
		VisualEffects->AddNumOption(0, LOCTEXT("VisualEffects_Low", "Low"));
		VisualEffects->AddNumOption(1, LOCTEXT("VisualEffects_Medium", "Medium"));
		VisualEffects->AddNumOption(2, LOCTEXT("VisualEffects_High", "High"));
		VisualEffects->AddNumOption(3, LOCTEXT("VisualEffects_Epic", "Epic"));
		VisualEffects->AddNumOption(4, LOCTEXT("VisualEffects_Cinematic", "Cinematic"));
		VisualEffects->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(VisualEffects);
		Graphics->AddSetting(VisualEffects);

		USettingValueDiscreteItem_Num* PostProcessing = NewObject<USettingValueDiscreteItem_Num>();
		PostProcessing->SetDevName(TEXT("PostProcessingSetting"));
		PostProcessing->SetDisplayName(LOCTEXT("SETTING_PostProcessing","Post Processing"));
		PostProcessing->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetPostProcessingQuality));
		PostProcessing->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetPostProcessingQuality));
		PostProcessing->AddNumOption(0, LOCTEXT("PostProcessing_Low", "Low"));
		PostProcessing->AddNumOption(1, LOCTEXT("PostProcessing_Medium", "Medium"));
		PostProcessing->AddNumOption(2, LOCTEXT("PostProcessing_High", "High"));
		PostProcessing->AddNumOption(3, LOCTEXT("PostProcessing_Epic", "Epic"));
		PostProcessing->AddNumOption(4, LOCTEXT("PostProcessing_Cinematic", "Cinematic"));
		PostProcessing->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(PostProcessing);
		Graphics->AddSetting(PostProcessing);

		USettingValueDiscreteItem_Num* Foliage = NewObject<USettingValueDiscreteItem_Num>();
		Foliage->SetDevName(TEXT("FoliageSetting"));
		Foliage->SetDisplayName(LOCTEXT("SETTING_Foliage","Foliage"));
		Foliage->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetFoliageQuality));
		Foliage->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetFoliageQuality));
		Foliage->AddNumOption(0, LOCTEXT("Foliage_Low", "Low"));
		Foliage->AddNumOption(1, LOCTEXT("Foliage_Medium", "Medium"));
		Foliage->AddNumOption(2, LOCTEXT("Foliage_High", "High"));
		Foliage->AddNumOption(3, LOCTEXT("Foliage_Epic", "Epic"));
		Foliage->AddNumOption(4, LOCTEXT("Foliage_Cinematic", "Cinematic"));
		Foliage->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(Foliage);
		Graphics->AddSetting(Foliage);

		USettingValueDiscreteItem_Num* Shading = NewObject<USettingValueDiscreteItem_Num>();
		Shading->SetDevName(TEXT("ShadingSetting"));
		Shading->SetDisplayName(LOCTEXT("SETTING_Shading","Shading"));
		Shading->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetShadingQuality));
		Shading->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetShadingQuality));
		Shading->AddNumOption(0, LOCTEXT("Shading_Low", "Low"));
		Shading->AddNumOption(1, LOCTEXT("Shading_Medium", "Medium"));
		Shading->AddNumOption(2, LOCTEXT("Shading_High", "High"));
		Shading->AddNumOption(3, LOCTEXT("Shading_Epic", "Epic"));
		Shading->AddNumOption(4, LOCTEXT("Shading_Cinematic", "Cinematic"));
		Shading->AddNeighbour(OverallQuality);
		OverallQuality->AddNeighbour(Shading);
		Graphics->AddSetting(Shading);
	}
	
	return Setting;
}

USettingItem* UAWeekSettingRegistry::RegisterAudioSetting()
{
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("AudioCategory"));
	Setting->SetDisplayName(LOCTEXT("NAME_AudioCategory","Audio"));

	USettingItemCategory* VolumeSetting = NewObject<USettingItemCategory>();
	VolumeSetting->SetDevName(TEXT("VolumeSetting"));
	VolumeSetting->SetDisplayName(LOCTEXT("SETTING_Volume","Volume"));
	Setting->AddSetting(VolumeSetting);
	
	USettingValueScalarItem* OverallSetting = NewObject<USettingValueScalarItem>();
	OverallSetting->SetDevName(TEXT("OverallVolume"));
	OverallSetting->SetDisplayName(LOCTEXT("SETTING_OverallVolume","Overall"));
	OverallSetting->SetMinValue(0.0f);
	OverallSetting->SetMaxValue(2.0f);
	OverallSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetOverallVolume());
	OverallSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetOverallVolume));
	OverallSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetOverallVolume));
	VolumeSetting->AddSetting(OverallSetting);

	USettingValueScalarItem* MusicSetting = NewObject<USettingValueScalarItem>();
	MusicSetting->SetDevName(TEXT("MusicVolume"));
	MusicSetting->SetDisplayName(LOCTEXT("SETTING_MusicVolume","MusicSetting"));
	MusicSetting->SetMinValue(0.0f);
	MusicSetting->SetMaxValue(2.0f);
	MusicSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetMusicVolume());
	MusicSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetMusicVolume));
	MusicSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetMusicVolume));
	VolumeSetting->AddSetting(MusicSetting);

	USettingValueScalarItem* SFXSetting = NewObject<USettingValueScalarItem>();
	SFXSetting->SetDevName(TEXT("SFXVolume"));
	SFXSetting->SetDisplayName(LOCTEXT("SETTING_SFXVolume","SFXSetting"));
	SFXSetting->SetMinValue(0.0f);
	SFXSetting->SetMaxValue(2.0f);
	SFXSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetSFXVolume());
	SFXSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetSFXVolume));
	SFXSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetSFXVolume));
	VolumeSetting->AddSetting(SFXSetting);

	USettingValueScalarItem* UISetting = NewObject<USettingValueScalarItem>();
	UISetting->SetDevName(TEXT("UIVolume"));
	UISetting->SetDisplayName(LOCTEXT("SETTING_UIVolume","UI"));
	UISetting->SetMinValue(0.0f);
	UISetting->SetMaxValue(2.0f);
	UISetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetUIVolume());
	UISetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetUIVolume));
	UISetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetUIVolume));
	VolumeSetting->AddSetting(UISetting);

	USettingValueScalarItem* AmbientSetting = NewObject<USettingValueScalarItem>();
	AmbientSetting->SetDevName(TEXT("AmbientVolume"));
	AmbientSetting->SetDisplayName(LOCTEXT("SETTING_AmbientVolume","Ambient"));
	AmbientSetting->SetMinValue(0.0f);
	AmbientSetting->SetMaxValue(2.0f);
	AmbientSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetAmbientVolume());
	AmbientSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetAmbientVolume));
	AmbientSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetAmbientVolume));
	VolumeSetting->AddSetting(AmbientSetting);
	
	return Setting;
}

USettingItem* UAWeekSettingRegistry::RegisterKeyboardAndMouseSetting()
{
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("KeyboardAndMouseCategory"));
	Setting->SetDisplayName(LOCTEXT("NAME_KeyboardAndMouseCategory","KeyboardAndMouse"));

	const UEnhancedInputLocalPlayerSubsystem* InputSubsystem = OwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	UEnhancedInputUserSettings* UserSettings = InputSubsystem->GetUserSettings();
	for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
	{
		const FGameplayTag& ProfileName = ProfilePair.Key;
		const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;

		for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
		{
			if (RowPair.Value.HasAnyMappings())
			{
				FPlayerMappableKeyQueryOptions Options = {};
				Options.KeyToMatch = EKeys::A;
				Options.bMatchBasicKeyTypes = true;

				for (const FPlayerKeyMapping& Mapping : RowPair.Value.Mappings)
				{
					if (!Profile->DoesMappingPassQueryOptions(Mapping, Options))
					{
						continue;
					}
					USettingValueItem_Input* InputSetting = NewObject<USettingValueItem_Input>();
					InputSetting->Init(Profile, RowPair.Value, Options, UserSettings);
					Setting->AddSetting(InputSetting);
				}
			}
		}
	}
		

	return Setting;
}
