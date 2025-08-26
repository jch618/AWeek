// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekCharacter.h"

// Sets default values
AAWeekCharacter::AAWeekCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mMWC = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
}

// Called when the game starts or when spawned
void AAWeekCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAWeekCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAWeekCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAWeekCharacter::VaultStart()
{
}

void AAWeekCharacter::VaultEnd()
{
}

