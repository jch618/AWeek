// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "MotionWarpingComponent.h"
#include "AWeekCharacter.generated.h"

UCLASS()
class AWEEK_API AAWeekCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAWeekCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMotionWarpingComponent> mMWC;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void VaultStart();
	virtual void VaultEnd();
	TObjectPtr<UMotionWarpingComponent> GetMWC()
	{
		return mMWC;
	}

};
