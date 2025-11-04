// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameUserSettings.h"

#include "AWeek/Audio/AWeekAudioSubsystem.h"

UAWeekGameUserSettings* UAWeekGameUserSettings::Get()
{
	return CastChecked<UAWeekGameUserSettings>(GEngine->GetGameUserSettings());
}

float UAWeekGameUserSettings::GetOverallVolume() const
{
	return OverallVolume;
}

void UAWeekGameUserSettings::SetOverallVolume(float InOverallVolume)
{
	OverallVolume = InOverallVolume;
	if (const UWorld* World = GEngine->GetCurrentPlayWorld())
	{
		UAWeekAudioSubsystem::Get(World)->SetSoundVolume(ESoundChannel::Overall, OverallVolume);
	}
}

float UAWeekGameUserSettings::GetMusicVolume() const
{
	return MusicVolume;
}

void UAWeekGameUserSettings::SetMusicVolume(float InMusicVolume)
{
	MusicVolume = InMusicVolume;
	if (const UWorld* World = GEngine->GetCurrentPlayWorld())
	{
		UAWeekAudioSubsystem::Get(World)->SetSoundVolume(ESoundChannel::Music, MusicVolume);
	}
}
