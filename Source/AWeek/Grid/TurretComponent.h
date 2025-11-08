// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurretComponent.generated.h"


class UArrowComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UTurretComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTurretComponent();
	UPROPERTY(EditInstanceOnly, Category="Turret|Setup")
	TWeakObjectPtr<USceneComponent> YawPivot;
	UPROPERTY(EditInstanceOnly, Category="Turret|Setup")
	TWeakObjectPtr<USceneComponent> PitchPivot;
	UPROPERTY(EditInstanceOnly, Category="Turret|Setup")
	TWeakObjectPtr<UArrowComponent> Muzzle;
	

	UFUNCTION(BlueprintCallable, Category="Turret|Setup")
	void SetTargets();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsActive = false;

	TArray<TWeakObjectPtr<APawn>> Targets;

	TWeakObjectPtr<APawn> CurrentTarget;

	void UpdateAim(float DT);
	bool IsValidTarget(const APawn* A) const;

	UPROPERTY(EditAnywhere, Category="Turret|Aim")
	float YawSpeedDegPerSec   = 180.f;
	UPROPERTY(EditAnywhere, Category="Turret|Aim")
	float PitchSpeedDegPerSec = 120.f;
	UPROPERTY(EditAnywhere, Category="Turret|Aim")
	float MinPitch = -10.f;
	UPROPERTY(EditAnywhere, Category="Turret|Aim")
	float MaxPitch =  60.f;


	float RetargetCooldown = 0.15f;
	float TimeSinceLastPick = 0.f;

	virtual void OnRegister() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddTarget(APawn* Target);
	void RemoveTarget(APawn* Target);

		
};
