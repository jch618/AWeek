// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameUserSettings.h"

#include "AWeek/Audio/AWeekAudioSubsystem.h"

UAWeekGameUserSettings* UAWeekGameUserSettings::Get()
{
	return CastChecked<UAWeekGameUserSettings>(GEngine->GetGameUserSettings());
}

bool UAWeekGameUserSettings::GetFullScreenModeTemp() const
{
	return IsFullScreenMode;
}

void UAWeekGameUserSettings::SetFullScreenModeTemp(bool InValue)
{
	IsFullScreenMode = InValue;
	if (IsFullScreenMode)
	{
		SetFullscreenMode(EWindowMode::Type::Fullscreen);
	}
	else
	{
		SetFullscreenMode(EWindowMode::Type::Windowed);
	}
}

float UAWeekGameUserSettings::GetOverallVolume() const
{
	return OverallVolume;
}

void UAWeekGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;
	if (const UWorld* World = GEngine->GetCurrentPlayWorld())
	{
		UAWeekAudioSubsystem::Get(World)->SetSoundVolume(ESoundChannel::Overall, OverallVolume);
	}
}

float UAWeekGameUserSettings::GetMusicVolume() const
{
	return MusicVolume;
}

void UAWeekGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;
	if (const UWorld* World = GEngine->GetCurrentPlayWorld())
	{
		UAWeekAudioSubsystem::Get(World)->SetSoundVolume(ESoundChannel::Music, MusicVolume);
	}
}

float UAWeekGameUserSettings::GetSFXVolume() const
{
	return SFXVolume;
}

void UAWeekGameUserSettings::SetSFXVolume(float InVolume)
{
	SFXVolume = InVolume;
	if (const UWorld* World = GEngine->GetCurrentPlayWorld())
	{
		UAWeekAudioSubsystem::Get(World)->SetSoundVolume(ESoundChannel::SFX, SFXVolume);
	}
}

float UAWeekGameUserSettings::GetUIVolume() const
{
	return UIVolume;
}

void UAWeekGameUserSettings::SetUIVolume(float InVolume)
{
	UIVolume = InVolume;
	if (const UWorld* World = GEngine->GetCurrentPlayWorld())
	{
		UAWeekAudioSubsystem::Get(World)->SetSoundVolume(ESoundChannel::UI, UIVolume);
	}
}

float UAWeekGameUserSettings::GetAmbientVolume() const
{
	return AmbientVolume;
}

void UAWeekGameUserSettings::SetAmbientVolume(float InVolume)
{
	AmbientVolume = InVolume;
	if (const UWorld* World = GEngine->GetCurrentPlayWorld())
	{
		UAWeekAudioSubsystem::Get(World)->SetSoundVolume(ESoundChannel::Ambient, AmbientVolume);
	}
}

float UAWeekGameUserSettings::GetGamma() const
{
	return GEngine->GetDisplayGamma();
}

void UAWeekGameUserSettings::SetGamma(float InGamma)
{
	GEngine->DisplayGamma = InGamma;
}
