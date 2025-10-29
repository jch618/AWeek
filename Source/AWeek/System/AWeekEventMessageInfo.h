// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekEventMessageInfo.generated.h"

USTRUCT(BlueprintType)
struct FStaminaChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Stamina;
	float Amount;
};

USTRUCT(BlueprintType)
struct FDayChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bIsDay = true;

	UPROPERTY(BlueprintReadWrite)
	int32 Day;
};

USTRUCT(BlueprintType)
struct FHungerChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Hunger;
	UPROPERTY(BlueprintReadWrite)
	float MaxHunger;
	UPROPERTY(BlueprintReadWrite)
	float Amount;
};