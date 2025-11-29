// Fill out your copyright notice in the Description page of Project Settings.

#include "AWeekDayWidget.h"
#include "../../System/DaySystem/AWeekDaySystem.h"
#include "AWeek/System/AWeekEventMessageInfo.h"

UAWeekDayWidget::UAWeekDayWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UAWeekDayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*--------------INIT--------------*/
	Label = Cast<UTextBlock>(GetWidgetFromName(TEXT("Label")));

	/*--------------EVENTMSSAGE--------------*/
	DayChangeHandle = UGameEventMessageSubsystem::Get(this).RegisterListener<FDayChangedMessage>(
		FGameplayTag::RequestGameplayTag(FName("Event.DayChanged")),
		[this](FGameplayTag Channel, const FDayChangedMessage& Msg)
		{
			Label->SetText(FText::AsNumber(Msg.Day));
		}
	);
}
