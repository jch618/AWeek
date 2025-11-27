// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SAWeekHitMarkerWidget : public SLeafWidget
{
	SLATE_BEGIN_ARGS(SAWeekHitMarkerWidget)
		:_HitOpacity(0.0f)
		,_DesiredSize(FVector2D::ZeroVector)
		{ 
		}
		SLATE_ATTRIBUTE(float, HitOpacity)
		SLATE_ARGUMENT(FVector2D, DesiredSize)
	SLATE_END_ARGS()

public:
	SAWeekHitMarkerWidget();
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	void Construct(const FArguments& InArgs, const TArray<FSlateBrush>& InHitMarkerImages);

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual bool ComputeVolatility() const override { return true; }

	void SetHitOpacity(float InHitOpacity);
private:
	TArray<FSlateBrush> HitMarkerImages;

	TAttribute<float> HitOpacity;

	FVector2D DesiredSize;
};

