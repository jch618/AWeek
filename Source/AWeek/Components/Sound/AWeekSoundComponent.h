// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekSoundComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UAWeekSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAWeekSoundComponent();

protected:
	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<USoundBase>> SoundMap;

public:
	void PlaySound(FName Name, FVector Location);

		
};
