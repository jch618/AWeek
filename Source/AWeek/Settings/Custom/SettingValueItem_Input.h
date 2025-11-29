// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingValueItem.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "SettingValueItem_Input.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API USettingValueItem_Input : public USettingValueItem
{
	GENERATED_BODY()

public:
	void Init(const UEnhancedPlayerMappableKeyProfile* KeyProfile, const FKeyMappingRow& MappingData, const FPlayerMappableKeyQueryOptions& InQueryOptions, UEnhancedInputUserSettings* NewInputSetting);
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Store() override;
	virtual void Restore() override;

	bool ChangeKey(int32 InSlot, FKey InKey);

	FText GetKeyText() const;
	FKey GetKey() const;
private:
	UPROPERTY()
	TObjectPtr<UEnhancedInputUserSettings> InputSetting;
	
	FName ActionMappingName;
	FPlayerMappableKeyQueryOptions QueryOptions;
	FGameplayTag ProfileIdentifier;
	
	FKey InitialKey;
	EPlayerMappableKeySlot InitialSlot;
};
