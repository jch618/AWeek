// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekAudioSubsystem.h"

#include "AWeekAudioSettings.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

void UAWeekAudioSubsystem::PostInitialize()
{
	Super::PostInitialize();
	
	
	if (const UAWeekAudioSettings* AudioSettings = GetDefault<UAWeekAudioSettings>())
	{
		if (UObject* AudioMix = AudioSettings->DefaultSoundMix.TryLoad())
		{
			if (USoundMix* SoundMix = Cast<USoundMix>(AudioMix))
			{
				DefaultSoundMix = SoundMix;
			}
		}
		
		RegisterSoundClass(ESoundChannel::Overall, AudioSettings->OverallClassPath);
		RegisterSoundClass(ESoundChannel::Music, AudioSettings->MusicClassPath);
		RegisterSoundClass(ESoundChannel::SFX, AudioSettings->SFXClassPath);
		RegisterSoundClass(ESoundChannel::Ambient, AudioSettings->AmbientClassPath);
		RegisterSoundClass(ESoundChannel::UI, AudioSettings->UIClassPath);
	}
}

void UAWeekAudioSubsystem::RegisterSoundClass(ESoundChannel SoundType, const FSoftObjectPath& SoftObjectPath)
{
	if (SoftObjectPath.IsValid())
	{
		if (USoundClass* SoundClass = Cast<USoundClass>(SoftObjectPath.TryLoad()))
		{
			SoundClasses.Add(SoundType, SoundClass);
		}
	}
}