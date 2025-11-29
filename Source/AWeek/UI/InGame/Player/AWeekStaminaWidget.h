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
	float CurrentTimer = 0;

	FGameEventMessageListenerHandle StaminaChangedHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeToFadeOut = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FadeDuration = 0.7f;

private:
	float mCurrentDelayTime = 0.0f;
	bool bStartFade = false;
	float mCurrentFadeOpacity = 1.0f;

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	
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
