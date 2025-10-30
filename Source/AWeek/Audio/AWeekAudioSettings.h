// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AWeekAudioSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "AWeekAudioSettings"))
class AWEEK_API UAWeekAudioSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "AudioSettings", meta = (AllowedClasses = "SoundMix"))
	FSoftObjectPath DefaultSoundMix;

	UPROPERTY(Config, EditAnywhere, Category = "AudioSettings", meta = (AllowedClasses = "SoundClass"))
	FSoftObjectPath OverallClassPath;

	UPROPERTY(Config, EditAnywhere, Category = "AudioSettings", meta = (AllowedClasses = "SoundClass"))
	FSoftObjectPath MusicClassPath;

	UPROPERTY(Config, EditAnywhere, Category = "AudioSettings", meta = (AllowedClasses = "SoundClass"))
	FSoftObjectPath SFXClassPath;

	UPROPERTY(Config, EditAnywhere, Category = "AudioSettings", meta = (AllowedClasses = "SoundClass"))
	FSoftObjectPath AmbientClassPath;

	UPROPERTY(Config, EditAnywhere, Category = "AudioSettings", meta = (AllowedClasses = "SoundClass"))
	FSoftObjectPath UIClassPath;
};
