// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekActivatableWidget.h"

UAWeekActivatableWidget::UAWeekActivatableWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UAWeekActivatableWidget::GetDesiredInputConfig() const
{
	switch (InputConfig)
	{
	case EAWeekWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EAWeekWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EAWeekWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, GameMouseCaptureMode);
	case EAWeekWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
