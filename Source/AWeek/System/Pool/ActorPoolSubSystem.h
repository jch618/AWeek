// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ActorPoolSubSystem.generated.h"

UCLASS()
class AWEEK_API UActorPoolSubSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category = "Pooling")
    AActor* GetPooledActor(TSubclassOf<AActor> ActorClass,
        FVector Location = FVector::ZeroVector,
        FRotator Rotation = FRotator::ZeroRotator,
        int32 NumToSpawnIfEmpty = 5);

    UFUNCTION(BlueprintCallable, Category = "Pooling")
    void ReturnActorToPool(AActor* Actor);

private:
    TMap<TSubclassOf<AActor>, TArray<AActor*>> ActorPools;

    void SpawnNewActors(TSubclassOf<AActor> ActorClass, int32 Count);
};
