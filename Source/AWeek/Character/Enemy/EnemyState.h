// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    Passive        UMETA(DisplayName = "Passive"),
    InCombat      UMETA(DisplayName = "InCombat"),
    Frozen   UMETA(DisplayName = "Frozen"),
    Dead       UMETA(DisplayName = "Dead"),
    Investigating       UMETA(DisplayName = "Investigating"),
    Destruction UMETA(DisplayName = "Destruction")
};