// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingArea.generated.h"

UCLASS()
class AWEEK_API ABuildingArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildingArea();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

UPROPERTY(VisibleAnywhere)
TObjectPtr<UStaticMeshComponent> Mesh;

UPROPERTY(VisibleAnywhere)
TObjectPtr<UMaterialInterface> Material;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActiveArea();

};
