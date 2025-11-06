// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Grid/PreviewObject.h"
#include "Components/BoxComponent.h"
#include "GridTriggerBoxComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AWEEK_API UGridTriggerBoxComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UGridTriggerBoxComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY();
	AActor* Owner;
	UPROPERTY()
	APreviewObject* OwnerClass;
	
	UStaticMeshComponent* StaticMesh;
	bool IsBuildingArea(const UPrimitiveComponent* Comp);

	

public:
	void SetBoxMesh(UStaticMeshComponent* StaticMeshComponent);

	
};
