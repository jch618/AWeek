// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAWeekCrossHairMarkerWidget.generated.h"

USTRUCT(BlueprintType)
struct FCircumferenceMarkerEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceUnits=deg))
	float PositionAngle = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ForceUnits=deg))
	float ImageRotationAngle = 0.0f;
};

/**
 * 
 */
class SAWeekCrossHairMarkerWidget : public SLeafWidget
{
	SLATE_BEGIN_ARGS(SAWeekCrossHairMarkerWidget)
			: _MarkerBrush(FCoreStyle::Get().GetBrush("Throbber.CircleChunk"))
			  , _Radius(48.0f)
		{
		}

		SLATE_ARGUMENT(const FSlateBrush*, MarkerBrush)
		SLATE_ARGUMENT(TArray<FCircumferenceMarkerEntry>, MarkerList)
		SLATE_ATTRIBUTE(float, Radius)
		SLATE_ATTRIBUTE(FSlateColor, ColorAndOpacity)
		SLATE_ARGUMENT(uint8, bReticleCornerOutsideSpreadRadius)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

	SAWeekCrossHairMarkerWidget();

	//~SWidget interface
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	virtual bool ComputeVolatility() const override { return true; }
	//~End of SWidget interface

	void SetRadius(float NewRadius);
	void SetMarkerList(TArray<FCircumferenceMarkerEntry>& NewMarkerList);

private:
	FSlateRenderTransform GetMarkerRenderTransform(const FCircumferenceMarkerEntry& Marker, const float BaseRadius,
	                                               const float HUDScale) const;

private:
	const FSlateBrush* MarkerBrush;
	TArray<FCircumferenceMarkerEntry> MarkerList;
	TAttribute<float> Radius;
	TAttribute<FSlateColor> ColorAndOpacity;
	bool bColorAndOpacitySet;

	uint8 bReticleCornerOutsideSpreadRadius : 1;
};
