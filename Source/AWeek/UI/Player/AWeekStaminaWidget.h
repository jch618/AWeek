// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AWeekUserWidget.h"
#include "AWeekStaminaWidget.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekStaminaWidget : public UAWeekUserWidget
{
	GENERATED_BODY()

public:
	UAWeekStaminaWidget(const FObjectInitializer& ObjectInitializer);

protected:
	TObjectPtr<UProgressBar> mProgress;
	class UWidgetAnimation* mAnim;
	FWidgetAnimationDynamicEvent mAnimFinishDelegate;

protected:
	virtual void NativeConstruct();
	
public:
	void PlayDisappearAnimation();
	void UpdateProgress(float CurrentStamina)
	{
		mProgress->SetPercent(CurrentStamina / 100);
	}
	void AfterUseStamina(float CurrentStamina);
	UFUNCTION()
	void HideWidget()
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
};
