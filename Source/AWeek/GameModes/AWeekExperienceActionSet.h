// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AWeekExperienceActionSet.generated.h"

class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UAWeekExperienceActionSet();

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
