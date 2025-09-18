// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavLinkComponent.h"
#include "NavLinkCustomComponent.h"
#include "Components/BoxComponent.h"
#include "LinkBoxComponent.generated.h"

/**
 * 
 */

class AGridPlacedActor;
class ANavLinkProxy;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AWEEK_API ULinkBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	ULinkBoxComponent();

	UPROPERTY(Transient) 
	UNavLinkCustomComponent* NLink;

	UPROPERTY(VisibleAnywhere)
	UNavLinkComponent* Link;

	UPROPERTY(VisibleAnywhere)
	ANavLinkProxy* LinkProxy;


	void SetLink();

protected:
	//virtual void OnComponentCreated() override;
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector LinkPosition = FVector::ZeroVector;
	FVector OwnerPosition = FVector::ZeroVector;
	void SetLinkPosition();

	bool bOverlap = false;
	AGridPlacedActor* OverlapStructure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGridPlacedActor* OwnerClass;
	TArray<AActor*> PlacedActors;

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	void DebugDrawLink();
};
