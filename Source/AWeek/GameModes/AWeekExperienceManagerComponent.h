// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "AWeekExperienceManagerComponent.generated.h"

class UAWeekExperienceDefinition;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<const UAWeekExperienceDefinition> CurrentExperience;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void ServerSetCurrentExperience(const FPrimaryAssetId& ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
};
