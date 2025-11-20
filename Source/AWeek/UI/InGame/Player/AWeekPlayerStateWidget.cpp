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
	HPChangedHandle = UGameEventMessageSubsystem::Get(this).RegisterListener<FHPChangedHandle>(
		FGameplayTag::RequestGameplayTag(FName("Event.HPChanged")),
		[this](FGameplayTag Channel, const FHPChangedHandle& Msg)
		{
			HealthBar->SetPercent(Msg.HP / Msg.MaxHP);
		}
	);

	HungerChangedHandle = UGameEventMessageSubsystem::Get(this).RegisterListener<FHungerChangedMessage>(
		FGameplayTag::RequestGameplayTag(FName("Event.HungerChanged")),
		[this](FGameplayTag Channel, const FHungerChangedMessage& Msg)
		{
			HungerBar->SetPercent(Msg.Hunger / Msg.MaxHunger);
		}
	);
}
