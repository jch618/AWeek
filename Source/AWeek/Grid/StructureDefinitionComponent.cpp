// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/StructureDefinitionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UStructureDefinitionComponent::UStructureDefinitionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(this);
	// ...
	
}


// Called when the game starts
void UStructureDefinitionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStructureDefinitionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStructureDefinitionComponent::OnRegister()
{
	Super::OnRegister();

	AActor* Owner = GetOwner(); if (!Owner) { return; }
	if (!StaticMesh)
	{
		StaticMesh = NewObject<UStaticMeshComponent>(Owner, TEXT("Structure_Mesh"));
		StaticMesh->SetupAttachment(this);
		Owner->AddInstanceComponent(StaticMesh);
		if (MeshAsset) StaticMesh->SetStaticMesh(MeshAsset);
	}

	if (!BoxComponent)
	{
		BoxComponent = NewObject<UBoxComponent>(Owner, TEXT("Structure_Box"));
		BoxComponent->SetupAttachment(this);
		Owner->AddInstanceComponent(BoxComponent);
		BoxComponent->SetCollisionProfileName(TEXT("Overlap"));
	}
}


