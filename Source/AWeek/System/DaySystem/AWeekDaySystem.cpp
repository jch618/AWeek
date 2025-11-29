// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekDaySystem.h"

#include "GameEventMessageSubsystem.h"
#include "AWeek/System/AWeekEventMessageInfo.h"

AAWeekDaySystem::AAWeekDaySystem()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAWeekDaySystem::BeginPlay()
{
    Super::BeginPlay();

    if (Sun)
    {
        Sun->GetLightComponent()->SetIntensity(SunIntensity);
    }
}

void AAWeekDaySystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateLighting();
    float DayProgress = (DeltaTime / DayLengthInSeconds) * 24.f;
    TimeOfDay += DayProgress;
    if (TimeOfDay >= 24.f) TimeOfDay -= 24.f;
}

void AAWeekDaySystem::UpdateLighting()
{
    bool bIsDay = (TimeOfDay >= 6.f && TimeOfDay < 18.f);

    if (bDayChangeFlag != bIsDay)
    {
        bDayChangeFlag = bIsDay;

        if (bIsDay)
            Day++;

        FDayChangedMessage Msg;
        Msg.bIsDay = bIsDay;
        Msg.Day = Day;

        UGameEventMessageSubsystem::Get(this).BroadcastMessage(
            FGameplayTag::RequestGameplayTag(FName("Event.DayChanged")),
            Msg
        );
    }

    if (Sun)
    {
        float Pitch = (TimeOfDay / 24.f) * 360.f + 90.f;

        FRotator SunRotation = FRotator(Pitch, -90.f, 0.f);
        Sun->SetActorRotation(SunRotation);
    }
}