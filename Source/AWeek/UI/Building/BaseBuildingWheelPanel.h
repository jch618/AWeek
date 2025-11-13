// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/Building/BuildingWheelPanel.h"
#include "BaseBuildingWheelPanel.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UBaseBuildingWheelPanel : public UBuildingWheelPanel
{
	GENERATED_BODY()
public:
	virtual void ActiveWheel() override;
};
