// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekHitMarkerWidget.h"

#include "SAWeekHitMarkerWidget.h"
#include "AWeek/System/GameEventMessageSubsystem.h"

void UAWeekHitMarkerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* World = GetWorld())
	{
		HitHandle = UGameEventMessageSubsystem::Get(this).RegisterListener<FEmptyPayload>(
			FGameplayTag::RequestGameplayTag(TEXT("Event.Hit.Enemy")),
			[this](FGameplayTag Channel, FEmptyPayload Payload)
			{
				this->HandleHitMessage();
			}
			);
	}
	
}

void UAWeekHitMarkerWidget::NativeDestruct()
{
	if (HitHandle.IsValid())
	{
		HitHandle.Unregister();
	}
	Super::NativeDestruct();
}

void UAWeekHitMarkerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (HitDuration > KINDA_SMALL_NUMBER)
	{
		HitOpacity = FMath::Clamp(HitOpacity - (InDeltaTime / HitDuration), 0.f, 1.f);
		SynchronizeProperties();
	}
}

void UAWeekHitMarkerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	HitMarkerWidget->SetHitOpacity(HitOpacity);
}

TSharedRef<SWidget> UAWeekHitMarkerWidget::RebuildWidget()
{
	HitMarkerWidget = SNew(SAWeekHitMarkerWidget, HitImages);
	return HitMarkerWidget.ToSharedRef();
}

void UAWeekHitMarkerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	HitMarkerWidget.Reset();
}

void UAWeekHitMarkerWidget::HandleHitMessage()
{
	HitOpacity = 1.0f;
}
