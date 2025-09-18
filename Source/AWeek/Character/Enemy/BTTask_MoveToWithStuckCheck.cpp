// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToWithStuckCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyState.h"



UBTTask_MoveToWithStuckCheck::UBTTask_MoveToWithStuckCheck()
{
	NodeName = TEXT("MoveTo With StuckCheck");
	bNotifyTick = true;
	bCreateNodeInstance = true;

	StuckTimeThreshold = 1.0f;
	MinProgressDistance = 10.f;
	CheckInterval = 0.25f;
	AcceptanceRadius = 50.f;

	LastPawnLocation = FVector::ZeroVector;
	TimeSinceNoProgress = 0.f;
	AccumulatedTick = 0.f;

	CachedAIController = nullptr;
	CachedPawn = nullptr;
}

EBTNodeResult::Type UBTTask_MoveToWithStuckCheck::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp,Warning,TEXT("[BTT_MoveToWithStuck] : ExecuteTask"))
	CachedAIController = OwnerComp.GetAIOwner();
	if (!CachedAIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTT_MoveToWithStuck] : There is No AIController"))
		return EBTNodeResult::Failed;
	}

	CachedPawn = CachedAIController->GetPawn();
	if (!CachedPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTT_MoveToWithStuck] : There is No AIPawn"))
		return EBTNodeResult::Failed;
	}

	LastPawnLocation = CachedPawn->GetActorLocation();
	LastTargetLocation = FVector::ZeroVector;
	TimeSinceNoProgress = 0.f;
	AccumulatedTick = 0.f;

	RequestMove();

	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToWithStuckCheck::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CachedAIController || !CachedPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AccumulatedTick += DeltaSeconds;
	if (AccumulatedTick < CheckInterval)
		return;

	AccumulatedTick = 0.f;

	float DistSq = FVector::DistSquared(CachedPawn->GetActorLocation(), LastPawnLocation);
	LastPawnLocation = CachedPawn->GetActorLocation();

	if (DistSq < FMath::Square(MinProgressDistance))
	{
		TimeSinceNoProgress += CheckInterval;

		if (TimeSinceNoProgress >= StuckTimeThreshold)
		{
			CachedAIController->StopMovement();
			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			if (BBComp)
			{
				BBComp->SetValueAsEnum(FName("State"), (uint8)EEnemyState::Destruction);
			}

			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}
	}
	else
	{
		TimeSinceNoProgress = 0.f;
	}

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
		return;
	UObject* TargetObj = BBComp->GetValueAsObject(GetSelectedBlackboardKey());
	if (TargetObj == nullptr)
		return;
	AActor* TargetActor = Cast<AActor>(TargetObj);

	FVector CurrentTargetLocation = TargetActor->GetActorLocation();
	if (FVector::DistSquared(CurrentTargetLocation, LastTargetLocation) > FMath::Square(10.f))
	{
		RequestMove();
		LastTargetLocation = CurrentTargetLocation;
	}

	if (FVector::Dist(CachedPawn->GetActorLocation(), CurrentTargetLocation) <= AcceptanceRadius)
	{
		if (CachedAIController)
			CachedAIController->StopMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTask_MoveToWithStuckCheck::RequestMove()
{
	if (!CachedAIController || !CachedPawn)
		return;

	UBlackboardComponent* BBComp = CachedAIController->GetBlackboardComponent();
	if (!BBComp)
		return;

	UObject* TargetObject= BBComp->GetValueAsObject(GetSelectedBlackboardKey());
	if (TargetObject == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTT_MoveToWithStuck] : There is No TargetObject"))
		return;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);
	FAIMoveRequest MoveReq;
	MoveReq.SetGoalActor(TargetActor);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius);
	MoveReq.SetUsePathfinding(true);

	FNavPathSharedPtr NavPath;
	CachedAIController->MoveTo(MoveReq, &NavPath);
}
