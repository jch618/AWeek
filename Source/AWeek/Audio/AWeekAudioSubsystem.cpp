// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekAudioSubsystem.h"

#include "AWeekAudioSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"

UAWeekAudioSubsystem* UAWeekAudioSubsystem::Get(const UWorld* World)
{
	return IsValid(World) == true ? World->GetSubsystem<UAWeekAudioSubsystem>() : nullptr;
}

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

void UAWeekAudioSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	check(DefaultSoundMix);

	UGameplayStatics::SetBaseSoundMix(&InWorld, DefaultSoundMix);
	UGameplayStatics::PushSoundMixModifier(&InWorld, DefaultSoundMix);
}

void UAWeekAudioSubsystem::SetSoundVolume(const ESoundChannel Channel, const float Volume)
{
	if (const TObjectPtr<USoundClass>* SoundClass = SoundClasses.Find(Channel))
	{
		UGameplayStatics::SetSoundMixClassOverride(
			GetWorld(),
			DefaultSoundMix,
			SoundClass->Get(),
			Volume,
			1.0f,
			0.0f,
			true);
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
