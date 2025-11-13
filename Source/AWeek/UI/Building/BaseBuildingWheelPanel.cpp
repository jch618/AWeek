// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Building/BaseBuildingWheelPanel.h"

#include "BuildingSelectWidget.h"

void UBaseBuildingWheelPanel::ActiveWheel()
{
	if (index == SectionCount)index = 0;
	UE_LOG(LogTemp, Log, TEXT("Base Building select widget click action %d"), index);
	BuildingSelectWidget->ChangeWheel(index+1);
}
