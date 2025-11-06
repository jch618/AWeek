// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "AWeekGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	static UAWeekGameUserSettings* Get();

	UFUNCTION()
	bool GetFullScreenModeTemp() const;

	UFUNCTION()
	void SetFullScreenModeTemp(bool InValue);

	UFUNCTION()
	float GetMouseSensitivityX() const { return MouseSensitivityX; }
	UFUNCTION()
	void SetMouseSensitivityX(const float InValue) { MouseSensitivityX = InValue; }

	UFUNCTION()
	float GetMouseSensitivityY() const { return MouseSensitivityY; }
	UFUNCTION()
	void SetMouseSensitivityY(const float InValue) { MouseSensitivityY = InValue; }
	
	UFUNCTION()
	float GetOverallVolume() const;
	UFUNCTION()
	void SetOverallVolume(float InOverallVolume);

	UFUNCTION()
	float GetMusicVolume() const;
	UFUNCTION()
	void SetMusicVolume(float InMusicVolume);
	
private:
	UPROPERTY(Config)
	bool IsFullScreenMode = true;
	
	UPROPERTY(Config)
	float MouseSensitivityX = 1.0f;

	UPROPERTY(Config)
	float MouseSensitivityY = 1.0f;
	
	UPROPERTY(Config)
	float OverallVolume = 1.0f;

	UPROPERTY(Config)
	float MusicVolume = 1.0f;
};
