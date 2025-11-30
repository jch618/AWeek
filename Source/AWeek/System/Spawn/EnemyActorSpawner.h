// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekActorSpawner.h"
#include "../../Character/Enemy/BaseEnemy.h"
#include "GameEventMessageSubsystem.h"
#include "../../System/AWeekEventMessageInfo.h"
#include "EnemyActorSpawner.generated.h"


UCLASS()
class AWEEK_API AEnemyActorSpawner : public AAWeekActorSpawner
{
    GENERATED_BODY()

public:
    AEnemyActorSpawner();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
    FGameEventMessageListenerHandle DayChangedListenerHandle;

    UPROPERTY()
    TArray<class ABaseEnemy*> SpawnedEnemies;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    int32 MaxActiveEnemies = 10;

protected:
    UFUNCTION()
    void OnDayChanged(const FDayChangedMessage& Msg);

    void SpawnEnemiesFromPool();

public:
    void UnregisterSpawnedEnemy(class ABaseEnemy* Enemy);

private:
    UPROPERTY()
    class UActorPoolSubSystem* ActorPoolSubsystem;
    UPROPERTY()
    class UCameraVisibilitySubsystem* VisibilitySubsystem;
};
