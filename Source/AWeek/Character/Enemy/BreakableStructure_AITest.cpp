// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableStructure_AITest.h"
#include "Components/StaticMeshComponent.h"
#include "NavModifierComponent.h"
#include "NavAreas/NavArea_Obstacle.h"

// Sets default values
ABreakableStructure_AITest::ABreakableStructure_AITest()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Cube Mesh
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMesh->SetupAttachment(RootComponent);

    // NavModifier
    NavModifier = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifier"));
    NavModifier->SetAreaClass(UNavArea_Obstacle::StaticClass());

}

void ABreakableStructure_AITest::StructureBreak()
{
    if (NavModifier)
    {
        NavModifier->SetAreaClass(nullptr); 
    }
    Destroy();
}

// Called when the game starts or when spawned
void ABreakableStructure_AITest::BeginPlay()
{
	Super::BeginPlay();
    if (StaticMesh && NavModifier)
    {
        FVector BoxExtent = StaticMesh->Bounds.BoxExtent;
        FVector Scale = StaticMesh->GetComponentScale();
        NavModifier->FailsafeExtent = BoxExtent * Scale;
    }
}

// Called every frame
void ABreakableStructure_AITest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

