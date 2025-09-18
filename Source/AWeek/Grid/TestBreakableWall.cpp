// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TestBreakableWall.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "TimerManager.h"
#include "Field/FieldSystemObjects.h"


// Sets default values
ATestBreakableWall::ATestBreakableWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GeoComp = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeoComp"));
	RootComponent = GeoComp;

}

// Called when the game starts or when spawned
void ATestBreakableWall::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimerForNextTick([this]
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ATestBreakableWall::BreakWall, 3.0f, false);
	});
	
}

// Called every frame
void ATestBreakableWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestBreakableWall::BreakWall()
{
	if (GeoComp)
	{
		GeoComp->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_LinearVelocity, nullptr, NewObject<URadialFalloff>());
	}
	
}


