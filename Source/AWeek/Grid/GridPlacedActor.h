// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BuildingData.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "AWeek/System/IDamageAble.h"
#include "GameFramework/Actor.h"
#include "GridPlacedActor.generated.h"


class UGeometryCollection;
class UGeometryCollectionComponent;
UCLASS()
class AWEEK_API AGridPlacedActor : public AActor, public IDamageAble
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridPlacedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	UPROPERTY(VisibleAnywhere, Category="Components")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMesh* MeshApply;

	UPROPERTY(EditAnywhere, Category="Components")
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere, Category="Components")
	UGeometryCollectionComponent* GeoComponent = nullptr;
	UPROPERTY(EditAnywhere, Category="Components")
	UGeometryCollection* GeoAsset = nullptr;

	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMeshComponent* BoxMesh;
	UPROPERTY(EditAnywhere, Category="Components")
	UStaticMesh* BoxMeshApply;
	UPROPERTY(EditAnywhere, Category="Components")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Category="BuildingStat")
	FBuildingStat BuildingStat;
	UPROPERTY(EditAnywhere, Category="BuildingStat")
	FBuildingSize BuildingSize;

	UPROPERTY(EditAnywhere, Category="Components")
	FVector2D SharedXY = FVector2D::ZeroVector;
	UPROPERTY(EditAnywhere, Category="Components")
	float GroundZ = 0.0f;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category="Building")
	TArray<class AGridPlacedActor*> GridPlacedActors;

	static FORCEINLINE float SafeDiv(float A, float B)
	{
		return A / FMath::Max(B, KINDA_SMALL_NUMBER);
	}

	static void MatchBoxToMesh(UBoxComponent* BoxComponent_, UStaticMeshComponent* BoxMesh_);

	UFUNCTION(BlueprintCallable, Category="Building|Fracture" /*, meta=(BlueprintAuthorityOnly="true")*/)
	void BrokeStructure();

	
	void Rebuild();
	void CleanupAfterBreak();

	//나중에 옮기기
	float GridSize = 100.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	void Damage(float Damage);

	bool bActive = true;

	FVector WorldTopPosition = FVector::ZeroVector;

	/*UFUNCTION(BlueprintCallable, Category = "Build")
	bool PlaceAtSlot(int32 SlotIndex, TSubclassOf<class AGridPlacedActor> ActorClass);
	
	UFUNCTION(BlueprintCallable, Category = "Build")
	bool RemoveFromSlot(int32 SlotIndex);*/

	void NotifyChildDestroyed(class AGridPlacedActor* ChildActor, int32 SlotIndex);
	void BuildActor(AGridPlacedActor* Actor);

	FVector GetLinkWorldPosition();

	UPROPERTY()
	AGridPlacedActor* ParentGridPlacedActor;

};
