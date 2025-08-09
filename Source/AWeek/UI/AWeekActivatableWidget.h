// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "AWeekActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EAWeekWidgetInputMode: uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu,
};

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UAWeekActivatableWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	UPROPERTY(EditDefaultsOnly, Category= Input)
	EAWeekWidgetInputMode InputConfig = EAWeekWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently; 
};
