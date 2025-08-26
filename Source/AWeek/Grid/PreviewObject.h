// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingData.h"
#include "GridPlacedActor.h"
#include "PreviewObject.generated.h"

class UGridTriggerBoxComponent;

UCLASS()
class AWEEK_API APreviewObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APreviewObject();

	UPROPERTY()
	bool bActiveActor = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Components")
	USceneComponent* Root;

	
	
	


	
	UPROPERTY(VisibleAnywhere, Category="MeshComponent")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category="MeshComponent")
	UStaticMesh* MeshApply;

	UPROPERTY(EditAnywhere, Category="MeshComponent")
	UStaticMeshComponent* BoxMesh;
	UPROPERTY(EditAnywhere, Category="MeshComponent")
	UStaticMesh* BoxMeshApply;
	

	UPROPERTY(EditAnywhere, Category="Preview")
	UMaterialInterface* TrueMaterial;
	UPROPERTY(EditAnywhere, Category="Preview")
	UMaterialInterface* FalseMaterial;
	UPROPERTY(EditAnywhere, Category="Preview")
	UMaterialInterface* ActiveMaterial;

	UPROPERTY(EditAnywhere, Category="BuildObject")
	TSubclassOf<AActor> BuildToPlace;
	bool bCanPlace = true;

	UPROPERTY(EditAnywhere, Category="BuildingSize")
	FBuildingSize BuildingSize;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UGridTriggerBoxComponent* BoxComponent;
	
	virtual void OnConstruction(const FTransform& Transform) override;

	static void SetMeshToGridCells(UStaticMeshComponent* Comp, const FIntVector& Cells, float Unit);
	static void SetStaticMeshToWorldSize(UStaticMeshComponent* Comp, const FVector& TargetSize);

	static void MatchBoxToMesh(UBoxComponent* BoxComponent_, UStaticMeshComponent* BoxMesh_);
	//static void SetStaticMeshToWorldSize(UStaticMeshComponent* Comp, const FVector& TargetSize);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CheckMaterial(bool value);
	bool PlaceActor(AGridPlacedActor* ParentGridPlacedActor);
	void SetActive(bool bValue);
};
