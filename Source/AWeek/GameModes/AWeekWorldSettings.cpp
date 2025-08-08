// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWorldSettings.h"

#include "AWeek/AWeekLogChannels.h"
#include "Engine/AssetManager.h"

FPrimaryAssetId AAWeekWorldSettings::GetDefaultGameplayExperience() const
{
	FPrimaryAssetId Result;
	if (!DefaultGameplayExperience.IsNull())
	{
		Result = UAssetManager::Get().GetPrimaryAssetIdForPath(DefaultGameplayExperience.ToSoftObjectPath());
    
		if (!Result.IsValid())
		{
			UE_LOG(LogAWeek, Error, TEXT("%s.DefaultGameplayExperience is %s load failed)"),
				*GetPathNameSafe(this), *DefaultGameplayExperience.ToString());
		}
	}
	return Result;
}
