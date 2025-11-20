// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingData.h"
#include "GridPlacedActor.h"
#include "Components/BoxComponent.h"
#include "PreviewObject.generated.h"

class UGridTriggerBoxComponent;
class UBoxComponent;
class AGridPlacedActor;
UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable)
class AWEEK_API APreviewObject : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	APreviewObject();

	UPROPERTY(EditAnywhere, Category="PlaceActor")
	AGridPlacedActor* GridPlaced;

	bool PlaceActor(AGridPlacedActor* ParentGridPlacedActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, Category="MeshComponent")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, Category="MeshComponent")
	UStaticMesh* Mesh;

	UPROPERTY(VisibleAnywhere, Category="BoxComponent")
	UStaticMeshComponent* BoxMesh;
	UPROPERTY(EditAnywhere, Category="BoxComponent")
	UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere, Category="BoxComponent")
	UStaticMesh* BoxMeshAsset;
	UPROPERTY(EditAnywhere, Category="Preview|Size")
	bool bSizeDrivenByMesh = true;

	//GirdSize는 나중에 확정지을것. 여기서는 아닐듯
	UPROPERTY(EditAnywhere, Category="BoxComponent")
	float GridSize = 100.f;
	UPROPERTY(EditAnywhere, Category="BoxComponent")
	FVector StructureSize = FVector(1.f,1.f,1.f);
	//에디터에서 StructureSize 변경시 거기에 맞게 BoxComponent, BoxMesh 사이즈 변경 함수
	void Rebuild();

	
	UPROPERTY(EditAnywhere, Category="Preview")
	UMaterialInterface* TrueMaterial;
	UPROPERTY(EditAnywhere, Category="Preview")
	UMaterialInterface* FalseMaterial;

	UPROPERTY(EditAnywhere, Category="BuildObject")
	TSubclassOf<AActor> BuildToPlace;
	bool bCanPlace = true;

	UPROPERTY(EditAnywhere, Category = "Visual")
	bool bBottomAlign = true;

	void SyncBoxComponentToBoxMesh();

	bool bOnBuildingArea = true;

	TArray<UPrimitiveComponent*> BlockObjects;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CheckMaterial(bool value);

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						   bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
/*public:	
	// Sets default values for this actor's properties
	APreviewObject();

	UPROPERTY(EditAnywhere, Category="PlaceActor")
	AGridPlacedActor* GridPlaced;

	bool PlaceActor(AGridPlacedActor* ParentGridPlacedActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category="MeshComponent")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category="MeshComponent")
	UStaticMesh* Mesh;
	
	UPROPERTY(EditAnywhere, Category="Preview")
	UMaterialInterface* TrueMaterial;
	UPROPERTY(EditAnywhere, Category="Preview")
	UMaterialInterface* FalseMaterial;

	UPROPERTY(EditAnywhere, Category="BuildObject")
	TSubclassOf<AActor> BuildToPlace;
	bool bCanPlace = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CheckMaterial(bool value);
	
	


	
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
	
	bool PlaceActor(AGridPlacedActor* ParentGridPlacedActor);
	void SetActive(bool bValue);*/
};
