// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingRegistry.h"
#include "AWeekSettingRegistry.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AWEEK_API UAWeekSettingRegistry : public USettingRegistry
{
	GENERATED_BODY()
	
public:
	virtual void Init(ULocalPlayer* InLocalPlayer) override;

	virtual void Apply() override;
	virtual void Cancel() override;

protected:
	USettingItem* RegisterGameplaySetting();
	USettingItem* RegisterAudioSetting();

protected:
	UPROPERTY()
	TObjectPtr<USettingItem> GameplaySetting;
	
	UPROPERTY()
	TObjectPtr<USettingItem> AudioSetting;
};
