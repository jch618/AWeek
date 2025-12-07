// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Grid/GridPlacedActor.h"
#include "TrapGridPlacedActor.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API ATrapGridPlacedActor : public AGridPlacedActor
{
	GENERATED_BODY()
public:
	ATrapGridPlacedActor();
protected:
	FTimerHandle FlameDamageTimerhandle;

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Trap|Damage")
	float DamageInterval = 0.7f;
	UPROPERTY(EditAnywhere, Category="Trap|Damage")
	float DamagePerTick = 2.f;

	UPROPERTY(EditAnywhere, Category="Trap")
	float TrapHealth = 100.f;


	UPROPERTY()
	TSet<AActor*> ActorsInDamageBox;
	UPROPERTY(EditAnywhere)
	UBoxComponent* DamageBox;
	UPROPERTY(EditAnywhere, Category="Turret")
	TSubclassOf<AActor> TargetClass;

	UFUNCTION()
	void DamageBoxTimer();

	UFUNCTION()
	void OnDamageBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnDamageBoxEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};
