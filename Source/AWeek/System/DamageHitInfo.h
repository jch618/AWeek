// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "DamageHitInfo.generated.h"

USTRUCT(BlueprintType)
struct FDamageHitInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ImpactPoint = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ImpactNormal = FVector::UpVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UPrimitiveComponent> HitComponent = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BoneName = NAME_None;
};