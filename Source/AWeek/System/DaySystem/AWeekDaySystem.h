// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AWeekDaySystem.generated.h"

UCLASS()
class AWEEK_API AAWeekDaySystem : public AActor
{
	GENERATED_BODY()

public:
    AAWeekDaySystem();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    int32 Day = 0;
    bool bDayChangeFlag = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    float DayLengthInSeconds = 300.f;

    // 현재 시간 (0 ~ 24)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DaySystem")
    float TimeOfDay = 6.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    ADirectionalLight* Sun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    float SunIntensity = 4.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    int32 WinDay;

protected:
    void UpdateLighting();
    UFUNCTION(BlueprintImplementableEvent) 
    void Win();

};