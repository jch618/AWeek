// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AWeekLootItemData.generated.h"

USTRUCT(BlueprintType)
struct FAWeekLootItemEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ItemHandle;

	UPROPERTY(EditAnywhere)
	int32 MinQuantity = 1;

	UPROPERTY(EditAnywhere)
	int32 MaxQuantity = 1;

	UPROPERTY(EditAnywhere)
	float DropChance = 100.0f;  // 0-100%
};

USTRUCT()
struct AWEEK_API FAWeekLootItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FAWeekLootItemEntry> PossibleLootItemEntries;

	UPROPERTY(EditAnywhere)
	int32 MinItemsToDrop = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxItemsToDrop = 3;
};
