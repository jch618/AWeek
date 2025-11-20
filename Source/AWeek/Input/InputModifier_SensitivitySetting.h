// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputModifier_SensitivitySetting.generated.h"

class UAWeekGameUserSettings;
/**
 * 
 */
UCLASS()
class AWEEK_API UInputModifier_SensitivitySetting : public UInputModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, Category = "Sensitivity")
	FName AxisSettingNameX = NAME_None;

	UPROPERTY(EditInstanceOnly, Category = "Sensitivity")
	FName AxisSettingNameY = NAME_None;
protected:
	UPROPERTY()
	TObjectPtr<UAWeekGameUserSettings> UserSettings;

	TArray<const FProperty*> Properties;

	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
