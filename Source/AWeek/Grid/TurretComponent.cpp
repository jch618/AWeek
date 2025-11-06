// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TurretComponent.h"

#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

// Sets default values for this component's properties
UTurretComponent::UTurretComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTurretComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTurretComponent::OnRegister()
{
	Super::OnRegister();
	
}


// Called every frame
void UTurretComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	UpdateAim(DeltaTime);
}

void UTurretComponent::SetTargets()
{
	if (Targets.Num() == 0){bIsActive = false;}
	else if (CurrentTarget == nullptr)
	{
		bIsActive = true;
		CurrentTarget = Targets[0];
	}
}


void UTurretComponent::AddTarget(APawn* Target)
{
	Targets.Add(Target);
	SetTargets();
}

void UTurretComponent::RemoveTarget(APawn* Target)
{
	Targets.Remove(Target);
	if (CurrentTarget == nullptr)
	{
		SetTargets();
	}
	if (CurrentTarget == Target)
	{
		CurrentTarget = nullptr;
	}
}

void UTurretComponent::UpdateAim(float DT)
{
	
}
bool UTurretComponent::IsValidTarget(const APawn* A) const
{
	return false;
}


