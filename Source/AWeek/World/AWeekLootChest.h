// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/World/AWeekChest.h"
#include "AWeekLootChest.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API AAWeekLootChest : public AAWeekChest
{
	GENERATED_BODY()
	
public:
	void AWeekLootChest();

protected:
	UPROPERTY(EditAnywhere)
	float LifeSpan;

	UPROPERTY()
	FTimerHandle LifeSpanTimer;

	virtual void BeginPlay() override;

	UFUNCTION()
	void DestoryOnTimerExpired();
};
