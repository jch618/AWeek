// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTriggerBoxComponent.h"
#include "Engine/EngineTypes.h"

static const FName NAME_BuildingArea(TEXT("BuildingArea"));
static const FName NAME_BuildingTrigger(TEXT("BuildingTrigger"));

UGridTriggerBoxComponent::UGridTriggerBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	/*SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetGenerateOverlapEvents(true);

	SetCollisionResponseToAllChannels(ECR_Ignore);*/

	

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
		OwnerClass->CheckMaterial(true);
		return;
	}
	TArray<UPrimitiveComponent*> OverlappingComps;
	GetOverlappingComponents(OverlappingComps);
	static const FName TestTag(TEXT("Test1"));
	bool bHasTaggedComp = false;
	bool bBuildingArea = false;
	
	for (UPrimitiveComponent* Comp : OverlappingComps)
	{
		if (!Comp) continue;
		if (IsBuildingArea(Comp))
		{
			bBuildingArea = true;
			//UE_LOG(LogTemp, Log, TEXT("BuildingArea Enter!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
			//continue;
		}
		if (Comp->ComponentHasTag(TestTag) && bBuildingArea)
		{
			//UE_LOG(LogTemp, Log, TEXT("Overlapping component has tag Test1 : %s"), *Comp->GetName());
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

bool UGridTriggerBoxComponent::IsBuildingArea(const UPrimitiveComponent* Comp)
{
	return Comp&& (Comp->GetCollisionProfileName() == NAME_BuildingArea);
}




