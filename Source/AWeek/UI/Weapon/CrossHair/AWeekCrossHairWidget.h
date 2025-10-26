// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/Weapon/AWeekReticleWidget.h"
#include "AWeekCrossHairWidget.generated.h"

class UAWeekCrossHairMarkerWidget;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekCrossHairWidget : public UAWeekReticleWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetFinalSpreadAngle() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetScreenspaceMaxSpreadAngleRadius() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasFirstShotAccuracy() const;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UAWeekCrossHairMarkerWidget> CrossHairMarker;
};
