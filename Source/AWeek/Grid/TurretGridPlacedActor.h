// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Grid/GridPlacedActor.h"
#include "TurretGridPlacedActor.generated.h"

/**
 * 
 */
class USceneComponent;
class USphereComponent;
class UNiagaraComponent;
class UAudioComponent;
class USoundBase;
UCLASS()
class AWEEK_API ATurretGridPlacedActor : public AGridPlacedActor
{
	GENERATED_BODY()
public:
	ATurretGridPlacedActor();
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/*UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;*/

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretHead;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* YawPivot;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* MuzzelFlashFXL;
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* MuzzelFlashFXR;
	UPROPERTY(EditAnywhere)
	UAudioComponent* MuzzleSound;
	UPROPERTY(EditAnywhere)
	USoundBase* SoundBase;

	UPROPERTY(EditAnywhere)
	USphereComponent* DetectCapsule;

	UPROPERTY(EditAnywhere, Category="Turret")
	TSubclassOf<AActor> TargetClass;
	//GunRange
	UPROPERTY(EditAnywhere, Category="Turret|Trace")
	float TraceDistance = 1500.f;
	//LineTrace channel 
	UPROPERTY(EditAnywhere, Category="Turret|Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	
	FTimerHandle TraceTimerHandle;

	UPROPERTY(EditAnywhere, Category="Turret|Trace")
	float DamagePerTick = 20.f;

	UPROPERTY(EditAnywhere, Category="Turret")
	float RotateSpeed = 5.f;
	UPROPERTY(EditAnywhere, Category="Turret")
	float TurretHealth = 100.f;

	UPROPERTY(EditAnywhere, Category="Turret|Trace")
	float TraceInterval = 0.5f;

	UFUNCTION()
	void TurretLineTrace();

	UPROPERTY(EditAnywhere, Category="Turret")
	bool bYawOnly = true;
};
