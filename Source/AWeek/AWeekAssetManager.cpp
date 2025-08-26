// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekAssetManager.h"

UAWeekAssetManager::UAWeekAssetManager()
{
}


UAWeekAssetManager& UAWeekAssetManager::Get()
{
	UAWeekAssetManager* Singleton = Cast<UAWeekAssetManager>(GEngine->AssetManager);

	return *Singleton;
}

void UAWeekAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}

void UAWeekAssetManager::FinishInitialLoading()
{
	Super::FinishInitialLoading();
}

UDataTable* UAWeekAssetManager::FindDataTable(const FName& TableName) const
{
	FPrimaryAssetType AssetType = FPrimaryAssetType(TEXT("DataTable"));

	TArray<FPrimaryAssetId> AssetIds;
	GetPrimaryAssetIdList(AssetType, AssetIds);

	for (auto& AssetId : AssetIds)
	{
		FString AssetName = AssetId.PrimaryAssetName.ToString();

		if (AssetName == TableName.ToString())
		{
			FAssetData AssetData;

			GetPrimaryAssetData(AssetId, AssetData);

			UDataTable* Table = Cast<UDataTable>(AssetData.GetAsset());

			if (Table)
				return Table;

			else
			{
				Table = Cast<UDataTable>(GetPrimaryAssetObject(AssetId));

				return Table;
			}
		}
	}

	return nullptr;
}
