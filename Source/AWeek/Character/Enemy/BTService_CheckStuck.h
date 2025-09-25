// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckStuck.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AWEEK_API UBTService_CheckStuck : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_CheckStuck();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, Category = "StuckCheck")
	float StuckTimeThreshold;

	UPROPERTY(EditAnywhere, Category = "StuckCheck")
	float MinProgressDistance;

	UPROPERTY(EditAnywhere, Category = "StuckCheck")
	float CheckInterval;

	// 내부 상태
	FVector LastPawnLocation;
	float TimeSinceNoProgress;
	float AccumulatedTick;
	
};