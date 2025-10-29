// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMovementType : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Walking     UMETA(DisplayName = "Walking"),
    Jogging     UMETA(DisplayName = "Jogging"),
    Sprinting   UMETA(DisplayName = "Sprinting")
};
