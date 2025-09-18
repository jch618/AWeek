// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToWithStuckCheck.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AWEEK_API UBTTask_MoveToWithStuckCheck : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToWithStuckCheck();

	UPROPERTY(EditAnywhere, Category = "Move|Stuck")
	float StuckTimeThreshold;

	UPROPERTY(EditAnywhere, Category = "Move|Stuck")
	float MinProgressDistance;

	UPROPERTY(EditAnywhere, Category = "Move|Stuck")
	float CheckInterval;

	UPROPERTY(EditAnywhere, Category = "Move")
	float AcceptanceRadius;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FVector LastPawnLocation;
	FVector LastTargetLocation;
	float TimeSinceNoProgress;
	float AccumulatedTick;

	AAIController* CachedAIController;
	APawn* CachedPawn;

	void RequestMove();
};
