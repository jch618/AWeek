// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekExperienceDefinition.h"

#include "GameFeatureAction.h"

UAWeekExperienceDefinition::UAWeekExperienceDefinition(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer)
{
}

#if WITH_EDITORONLY_DATA
void UAWeekExperienceDefinition::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action: Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif
