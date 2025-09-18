// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "AWeekMainMenuStateComponent.generated.h"

class UCommonActivatableWidget;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekMainMenuStateComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSoftClassPtr<UCommonActivatableWidget> MainMenuClass;
};
