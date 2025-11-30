// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/Building/BuildingNotionPanel.h"
#include "BuildingSelectNotionPanel.generated.h"

/**
 * 
 */
class UBuildCostPill;
class UAWeekInventoryComponent;
class UCommonHierarchicalScrollBox;
struct FAWeekCost;
UCLASS()
class AWEEK_API UBuildingSelectNotionPanel : public UBuildingNotionPanel
{
	GENERATED_BODY()
public:
	virtual void UpdateData(const FAWeekBuildingData* Data = nullptr) override;
	virtual void ClearData() override;
	void RemoveItem();
	bool bCheck = false;
	bool bCreateCheck = false;
protected:
	UPROPERTY()
	TArray<UBuildCostPill*> Pills;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBuildCostPill> CostPillClass;
	UPROPERTY()
	UAWeekInventoryComponent* InventoryComponent = nullptr;
	UPROPERTY(meta=(BindWidget))
	UCommonHierarchicalScrollBox* HierBox;


	
	
	void UpdateItemCount();
	void ApplyText(FString BuildingText);
	void CreateCostPill(const FAWeekCost Cost);
	bool bInventoryCheck = false;
};
