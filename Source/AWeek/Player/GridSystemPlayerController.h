// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Grid/PreviewObject.h"
#include "AWeek/Grid/GridPlacedActor.h"
#include "AWeekPlayerController.h"
#include "GridSystemPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API AGridSystemPlayerController : public AAWeekPlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SnapPreviewToSurface();

	UPROPERTY(EditAnywhere, Category="Grid")
	float GridSize = 100.f;

	UPROPERTY(EditAnywhere, Category="Grid")
	TSubclassOf<AActor> BuildToPlaceObject;

	UPROPERTY(EditAnywhere, Category="Grid")
	APreviewObject* PreviewActor;
	UPROPERTY(VisibleAnywhere)
	AGridPlacedActor* ParentGridPlacedActor;
	

	UFUNCTION()
	void HandleLeftClick();
	UFUNCTION()
	void RotatePreviewActor();

	FVector GetSnappedLocation(const FVector& WorldLocation) const;
	FVector SnapToGrid(const FVector& Location);
};
