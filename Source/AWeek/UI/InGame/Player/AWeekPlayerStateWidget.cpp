// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerStateWidget.h"

UAWeekPlayerStateWidget::UAWeekPlayerStateWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UAWeekPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*--------------INIT--------------*/
	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	HungerBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HungerBar")));

	/*--------------EVENTMSSAGE--------------*/
	HungerChangedHandle = UGameEventMessageSubsystem::Get(this).RegisterListener<FHungerChangedMessage>(
		FGameplayTag::RequestGameplayTag(FName("Event.HungerChanged")),
		[this](FGameplayTag Channel, const FHungerChangedMessage& Msg)
		{
			HungerBar->SetPercent(Msg.Hunger / Msg.MaxHunger);
		}
	);
}
