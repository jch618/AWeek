// Fill out your copyright notice in the Description page of Project Settings.


#include "AIParkourLinkProxy.h"
#include "AIController.h"

AAIParkourLinkProxy::AAIParkourLinkProxy()
{
    bSmartLinkIsRelevant = true;
}

void AAIParkourLinkProxy::BeginPlay()
{
    Super::BeginPlay();
    OnSmartLinkReached.AddDynamic(this, &AAIParkourLinkProxy::HandleSmartLinkReached);
}

void AAIParkourLinkProxy::HandleSmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint)
{
    if (!MovingActor) return;

    if (MovingActor->Tags.Contains(FName("CanVault")))
    {
        ResumePathFollowing(MovingActor);
        return;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("%s does NOT have CanVault tag. Ignoring NavLink."), *MovingActor->GetName());

    if (AAIController* AICon = Cast<AAIController>(MovingActor->GetInstigatorController()))
    {
        AICon->StopMovement();
        AICon->MoveToLocation(DestinationPoint);
    }
}


