// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekExperienceManagerComponent.h"

#include "AWeekExperienceActionSet.h"
#include "AWeekExperienceDefinition.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"
#include "Engine/AssetManager.h"

void UAWeekExperienceManagerComponent::ServerSetCurrentExperience(const FPrimaryAssetId& ExperienceId)
{
	const UAssetManager& AssetManager = UAssetManager::Get();

	const FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
	TSubclassOf<UAWeekExperienceDefinition> AssetClass = Cast<UClass>(AssetPath.TryLoad());

	const UAWeekExperienceDefinition* Experience = GetDefault<UAWeekExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void UAWeekExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience != nullptr);

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());
	for (const TObjectPtr<UAWeekExperienceActionSet>& ActionSet: CurrentExperience->ActionSets)
	{
		if (ActionSet != nullptr)
		{
			BundleAssetList.Add(ActionSet->GetPrimaryAssetId());	
		}
	}

	TArray<FName> BundlesToLoad;

	const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
	const bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
	const bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
	if (bLoadClient)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
	}
	if (bLoadServer)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
	}

	
	TSharedPtr<FStreamableHandle> BundleLoadHandle = nullptr;
	if (BundleAssetList.Num() > 0)
	{
		BundleLoadHandle = UAssetManager::Get().ChangeBundleStateForPrimaryAssets(BundleAssetList.Array(), BundlesToLoad, {}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
	if (!BundleLoadHandle.IsValid() || BundleLoadHandle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		BundleLoadHandle->BindCompleteDelegate(OnAssetsLoadedDelegate);

		BundleLoadHandle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}
}

void UAWeekExperienceManagerComponent::OnExperienceLoadComplete()
{
	check(CurrentExperience != nullptr);

	UE_LOG(LogTemp, Warning, TEXT("On Experience Load Complete"));
	FGameFeatureActivatingContext Context;
	{
		const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (ExistingWorldContext)
		{
			Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
		}
	}

	auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
	{
		for (UGameFeatureAction* Action: ActionList)
		{
			if (Action)
			{
				Action->OnGameFeatureRegistering();
				Action->OnGameFeatureLoading();
				Action->OnGameFeatureActivating(Context);
			}
		}
	};

	ActivateListOfActions(CurrentExperience->Actions);

	for (const TObjectPtr<UAWeekExperienceActionSet>& ActionSet: CurrentExperience->ActionSets)
	{
		ActivateListOfActions(ActionSet->Actions);
	}
}
