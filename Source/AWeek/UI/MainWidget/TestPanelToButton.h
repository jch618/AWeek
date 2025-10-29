// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "TestPanelToButton.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UTestPanelToButton : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	void SetActive();
	
};
