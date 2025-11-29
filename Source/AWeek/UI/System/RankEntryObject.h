// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include"../../System/Score/RankSaveData.h"
#include "RankEntryObject.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API URankEntryObject : public UObject
{
	GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    FRankEntry Entry;

    // helper creator
    static URankEntryObject* CreateFrom(UObject* Outer, const FRankEntry& InEntry)
    {
        URankEntryObject* Obj = NewObject<URankEntryObject>(Outer ? Outer : (UObject*)GetTransientPackage());
        Obj->Entry = InEntry;
        return Obj;
    }
};
