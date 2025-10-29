// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EMovementType.h"
#include "EnemyStatData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType) 
struct AWEEK_API FEnemyStatData :public FTableRowBase
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMovementType, float> MoveSpeedMap;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SightRadius = 800;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LoseSightRadius = 1200;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionAngleDegrees = 60.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float  MaxAgeSeconds = 5.f; 


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AnimPlayRate = 1.f;
};
