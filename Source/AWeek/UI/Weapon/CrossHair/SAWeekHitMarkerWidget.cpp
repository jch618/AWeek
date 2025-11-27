// Copyright Epic Games, Inc. All Rights Reserved.

#include "SAWeekHitMarkerWidget.h"
#include "Styling/SlateBrush.h"

SAWeekHitMarkerWidget::SAWeekHitMarkerWidget()
{
}

int32 SAWeekHitMarkerWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const bool bDrawing = (HitOpacity.Get() > KINDA_SMALL_NUMBER);
	if (bDrawing)
	{
		const FLinearColor BaseTint = InWidgetStyle.GetColorAndOpacityTint();
		const float OpacityValue = HitOpacity.Get();
		const FLinearColor FinalTint(
			BaseTint.R,
			BaseTint.G,
			BaseTint.B,
			BaseTint.A * OpacityValue
		);

		const FVector2D WidgetSize = AllottedGeometry.GetLocalSize();

		for (const FSlateBrush& Brush : HitMarkerImages)
		{
			if (Brush.DrawAs == ESlateBrushDrawType::NoDrawType)
			{
				continue;
			}

			const FVector2D ImageSize = Brush.ImageSize;

			// 위젯 중앙에 배치
			const FVector2D DrawPos = (WidgetSize - ImageSize) * 0.5f;

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,    
				AllottedGeometry.ToPaintGeometry(ImageSize, FSlateLayoutTransform(DrawPos)),
				&Brush,
				ESlateDrawEffect::None,
				FinalTint
			);
		}

		// 이 레이어까지 사용했으니 다음 레이어 반환
		return LayerId + 1;
	}

	return LayerId;
}

void SAWeekHitMarkerWidget::Construct(const FArguments& InArgs, const TArray<FSlateBrush>& InHitMarkerImages)
{
	HitOpacity = InArgs._HitOpacity;
	HitMarkerImages = InHitMarkerImages;
	DesiredSize = InArgs._DesiredSize;
	for (const FSlateBrush& Brush: HitMarkerImages)
	{
		DesiredSize.X = FMath::Max(DesiredSize.X, Brush.ImageSize.X);
		DesiredSize.Y = FMath::Max(DesiredSize.Y, Brush.ImageSize.Y);
	}
}

FVector2D SAWeekHitMarkerWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return DesiredSize;
}

void SAWeekHitMarkerWidget::SetHitOpacity(float InHitOpacity)
{
	if (HitOpacity.Get() != InHitOpacity || HitOpacity.IsBound())
	{
		HitOpacity = InHitOpacity;
		Invalidate(EInvalidateWidgetReason::Paint);
	}
}
