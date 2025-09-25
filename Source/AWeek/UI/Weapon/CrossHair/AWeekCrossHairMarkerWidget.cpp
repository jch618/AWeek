// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekCrossHairMarkerWidget.h"

UAWeekCrossHairMarkerWidget::UAWeekCrossHairMarkerWidget(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
}

void UAWeekCrossHairMarkerWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	MarkerWidget->SetRadius(Radius);
	MarkerWidget->SetMarkerList(MarkerList);
}

TSharedRef<SWidget> UAWeekCrossHairMarkerWidget::RebuildWidget()
{
	MarkerWidget = SNew(SAWeekCrossHairMarkerWidget)
		.MarkerBrush(&MarkerImage)
		.Radius(Radius)
		.MarkerList(MarkerList);

	return MarkerWidget.ToSharedRef();
}

void UAWeekCrossHairMarkerWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MarkerWidget.Reset();
}

void UAWeekCrossHairMarkerWidget::SetRadius(float InRadius)
{
	Radius = InRadius;
	if (MarkerWidget.IsValid())
	{
		MarkerWidget->SetRadius(InRadius);
	}
}
