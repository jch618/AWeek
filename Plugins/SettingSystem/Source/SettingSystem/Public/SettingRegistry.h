// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingItem.h"
#include "UObject/NoExportTypes.h"
#include "SettingRegistry.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingRegistry : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_OneParam(USettingRegistry, FOnSettingChanged, USettingItem*);

	FOnSettingChanged OnSettingChanged;

public:
	virtual void Init(ULocalPlayer* InLocalPlayer);
	virtual void Apply();
	virtual void Cancel();

	void RegisterSetting(USettingItem* Setting);
	void InitSetting(USettingItem* Setting);
protected:
	void HandleSettingChanged(USettingItem* Setting);
	void HandleSettingApplied(USettingItem* Setting);
	
	UPROPERTY()
	TObjectPtr<ULocalPlayer> OwningLocalPlayer;

	UPROPERTY()
	TArray<TObjectPtr<USettingItem>> RootSettings;

	TMap<FObjectKey, TObjectPtr<USettingItem>> DirtySettings;
};
