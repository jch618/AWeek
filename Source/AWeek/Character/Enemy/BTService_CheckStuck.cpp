#include "BTService_CheckStuck.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckStuck::UBTService_CheckStuck()
{
	NodeName = TEXT("Check Stuck");

	bCreateNodeInstance = true;


	StuckTimeThreshold = 1.0f;
	MinProgressDistance = 10.f;
	CheckInterval = 0.25f;

	LastPawnLocation = FVector::ZeroVector;
	TimeSinceNoProgress = 0.f;
	AccumulatedTick = 0.f;
}

void UBTService_CheckStuck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return;

	APawn* Pawn = AICon->GetPawn();
	if (!Pawn) return;

	AccumulatedTick += DeltaSeconds;
	if (AccumulatedTick < CheckInterval)
		return;
	AccumulatedTick = 0.f;

	float DistSq = FVector::DistSquared(Pawn->GetActorLocation(), LastPawnLocation);
	LastPawnLocation = Pawn->GetActorLocation();

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return;

	bool bIsStuck = false;

	if (DistSq < FMath::Square(MinProgressDistance))
	{
		TimeSinceNoProgress += CheckInterval;
		if (TimeSinceNoProgress >= StuckTimeThreshold)
		{
			bIsStuck = true;
		}
	}
	else
	{
		TimeSinceNoProgress = 0.f;
	}

	BBComp->SetValueAsBool(GetSelectedBlackboardKey(), bIsStuck);
}

void UBTService_CheckStuck::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	LastPawnLocation = FVector::ZeroVector;
	TimeSinceNoProgress = 0.f;
	AccumulatedTick = 0.f;
}