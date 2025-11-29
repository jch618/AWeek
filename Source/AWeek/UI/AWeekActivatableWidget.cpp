// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekActivatableWidget.h"

UAWeekActivatableWidget::UAWeekActivatableWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

bool UAWeekActivatableWidget::NativeOnHandleBackAction()
{
	return Super::NativeOnHandleBackAction();
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
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
	case EAWeekWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}
