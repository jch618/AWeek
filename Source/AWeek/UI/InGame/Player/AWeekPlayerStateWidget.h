// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../System/GameEventMessageSubsystem.h"

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

	FGameEventMessageListenerHandle HungerChangedHandle;

protected:
	virtual void NativeConstruct() override;
};
