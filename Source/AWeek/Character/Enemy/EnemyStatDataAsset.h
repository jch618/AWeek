// EnemyStatDataAsset.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EMovementType.h"
#include "EnemyStatData.h" // FEnemyStatData Á¤ÀÇ Æ÷ÇÔ
#include "EnemyStatDataAsset.generated.h"

UCLASS(BlueprintType)
class AWEEK_API UEnemyStatDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    // ³· ½ºÅÈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FEnemyStatData DayData;

    // ¹ã ½ºÅÈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FEnemyStatData NightData;
};
