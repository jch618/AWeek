// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEventMessageSubsystem.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "AWeekPlayerStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UAWeekPlayerStateWidget(const FObjectInitializer& ObjectInitializer);

protected:
	TObjectPtr<UProgressBar> HealthBar;
	TObjectPtr<UProgressBar> HungerBar;

	FGameEventMessageListenerHandle HPChangedHandle;
	FGameEventMessageListenerHandle HungerChangedHandle;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnRemovedFromFocusPath(FFocusEvent InFocusEvent);

public:
	void SetHPPercent(float CurrentHP, float MaxHP)
	{
		HealthBar->SetPercent(CurrentHP/MaxHP);
	}
};
