// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEventMessageSubsystem.h"

#include "Components/ProgressBar.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

#include "AWeekStaminaWidget.generated.h"

UCLASS()
class AWEEK_API UAWeekStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAWeekStaminaWidget(const FObjectInitializer& ObjectInitializer);

protected:
	TObjectPtr<UProgressBar> Progress;
	class UWidgetAnimation* FadeOutAnim;
	FTimerHandle FadeOutTimer;
	float TimeToFadeOut = 2;

	FGameEventMessageListenerHandle StaminaChangedHandle;

protected:
	virtual void NativeConstruct();
	
public:
	void UpdateProgress(float Stamina, bool bDecrease);
	
	UFUNCTION()
	void ShowWidget()
	{
		SetVisibility(ESlateVisibility::Visible);
	}

	UFUNCTION()
	void HideWidget()
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}

};
