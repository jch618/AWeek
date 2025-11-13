// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekActorSpawner.h"
#include"../../System/GameEventMessageSubsystem.h"
#include"../../System/AWeekEventMessageInfo.h"
#include "EnemyActorSpawner.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API AEnemyActorSpawner : public AAWeekActorSpawner
{
	GENERATED_BODY()

public:
	AEnemyActorSpawner();

protected:
	virtual void BeginPlay() override;
private:
	FGameEventMessageListenerHandle DayChangedListenerHandle;
protected:
	UFUNCTION()
	void OnDayChanged(const FDayChangedMessage& Msg);
	void SpawnEnemiesFromPool();
private :
	UPROPERTY()
	class UActorPoolSubSystem* ActorPoolSubsystem;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
};
