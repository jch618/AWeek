// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "StructureDefinitionComponent.generated.h"


UCLASS( ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class AWEEK_API UStructureDefinitionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStructureDefinitionComponent();

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* BoxComponent;


	UPROPERTY(EditAnywhere, Category = "Structure")
	TObjectPtr<UStaticMesh> MeshAsset;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
