// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAWeekCrossHairMarkerWidget.h"
#include "Blueprint/UserWidget.h"
#include "AWeekCrossHairMarkerWidget.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekCrossHairMarkerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAWeekCrossHairMarkerWidget(const FObjectInitializer& ObjectInitializer);

	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION(BlueprintCallable)
	void SetRadius(float InRadius);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCircumferenceMarkerEntry> MarkerList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Radius = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSlateBrush MarkerImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bReticleCornerOutsideSpreadRadius : 1;

	TSharedPtr<SAWeekCrossHairMarkerWidget> MarkerWidget;
};
