// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSoundComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAWeekSoundComponent::UAWeekSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAWeekSoundComponent::PlaySound(FName Name, FVector Location)
{
	if (SoundMap[Name])
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundMap[Name], Location);
	}
}
