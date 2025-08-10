// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AWeekExperienceDefinition.generated.h"

class UGameFeatureAction;
class UAWeekExperienceActionSet;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UAWeekExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TObjectPtr<UAWeekExperienceActionSet>> ActionSets;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
