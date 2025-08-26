// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AWeekAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UAWeekAssetManager();

public:
	static UAWeekAssetManager& Get();
	virtual void StartInitialLoading();
	virtual void FinishInitialLoading();
	UDataTable* FindDataTable(const FName& TableName) const;

	template <typename T>
	T* FindDataTableRow(const FName& TableName, const FName& RowName) const
	{
        UDataTable* DataTable = FindDataTable(TableName);
        if (DataTable)
        {
            T* FoundRow = DataTable->FindRow<T>(RowName, TEXT(""));
            if (FoundRow == nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("DataTable '%s' was found, but Row '%s' was not."), *TableName.ToString(), *RowName.ToString());
            }
            return FoundRow;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("DataTable '%s' could NOT be found!"), *TableName.ToString());
        }
        return nullptr;
	}
};
