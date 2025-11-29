// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RankSaveData.generated.h"

USTRUCT(BlueprintType)
struct FRankEntry
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    int32 Score;

    UPROPERTY()
    int32 Day;

    UPROPERTY()
    int32 ZombieKillCount;

    UPROPERTY()
    FGuid EntryID;
};

UCLASS()
class AWEEK_API URankSaveData : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FRankEntry> RankList;
	
};
