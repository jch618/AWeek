// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/BuildingArea.h"

#include "Components/StaticMeshComponent.h"


// Sets default values
ABuildingArea::ABuildingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}

	/*Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);*/
	Mesh->SetCastShadow(false);
}

// Called when the game starts or when spawned
void ABuildingArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABuildingArea::ActiveArea(){

}

