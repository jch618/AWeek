// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTriggerBoxComponent.h"

UGridTriggerBoxComponent::UGridTriggerBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGridTriggerBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Log, TEXT("Trigger Component Alive"));
	Owner = GetOwner();
	if (Owner)
	{
		UE_LOG(LogTemp, Log, TEXT("GetOwner True"));
		OwnerClass = Cast<APreviewObject>(GetOwner());
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("GetOwner False"));
	}
}

void UGridTriggerBoxComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!OwnerClass)
	{
		OwnerClass = Cast<APreviewObject>(GetOwner());
		if (OwnerClass->bActiveActor)
		return;
	}
	TArray<UPrimitiveComponent*> OverlappingComps;
	GetOverlappingComponents(OverlappingComps);
	static const FName TestTag(TEXT("Test1"));
	bool bHasTaggedComp = false;
	for (UPrimitiveComponent* Comp : OverlappingComps)
	{
		if (!Comp) continue;

		if (Comp->ComponentHasTag(TestTag))
		{
			UE_LOG(LogTemp, Log, TEXT("Overlapping component has tag Test1 : %s"), *Comp->GetName());
			bHasTaggedComp = true;
			break;
		}
		
	}	
	if (OwnerClass)
	{
		/*if (bHasTaggedComp){UE_LOG(LogTemp, Log, TEXT("True"));}
		else{UE_LOG(LogTemp, Log, TEXT("false"));}*/
		OwnerClass->CheckMaterial(bHasTaggedComp);
	}
	/*TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	if (Actors.Num()>0)
	{
		for (AActor* Actor : Actors)
		{
			if (Actor->ActorHasTag("Test1"))
			{
				UE_LOG(LogTemp, Log, TEXT("Actor has Tag"));
				UE_LOG(LogTemp, Log, TEXT("Actor name is %s"), *Actor->GetName());
			}
			OwnerClass->CheckMaterial(Actor->ActorHasTag("Test1"));
		}

		
		
	}else{OwnerClass->CheckMaterial(false);}*/
}