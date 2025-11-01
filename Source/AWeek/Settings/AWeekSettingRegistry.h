// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingRegistry.h"
#include "AWeekSettingRegistry.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekSettingRegistry : public USettingRegistry
{
	GENERATED_BODY()
	
public:
	virtual void Init(ULocalPlayer* InLocalPlayer) override;

protected:
	USettingItem* RegisterAudioSetting();

protected:
	UPROPERTY()
	TObjectPtr<USettingItem> AudioSetting;
};
