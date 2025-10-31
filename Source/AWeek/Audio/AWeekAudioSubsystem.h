// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AWeekAudioSubsystem.generated.h"

class UAWeekAudioSettings;

UENUM()
enum class ESoundChannel: uint8
{
	Overall,
	Music,
	SFX,
	Ambient,
	UI,
};
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekAudioSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UAWeekAudioSubsystem* Get(const UWorld* World);
	
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	void SetSoundVolume(ESoundChannel Channel, float Volume = 1.0f);
	
private:
	void RegisterSoundClass(ESoundChannel SoundType, const FSoftObjectPath& SoftObjectPath);

	UPROPERTY(Transient)
	TObjectPtr<USoundMix> DefaultSoundMix;
	
	UPROPERTY(Transient)
	TMap<ESoundChannel, TObjectPtr<USoundClass>> SoundClasses;
};
