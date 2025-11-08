// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/MainWidget/MainWidgetPanel.h"
#include "BuildingCraftPanel.generated.h"

/**
 * 
 */
class UCommonHierarchicalScrollBox;
class UCommonLazyImage;
class UCommonTextBlock;
class USizeBox;
struct FAWeekBuildingData;
struct FAWeekCost;
class UBuildCostPill;
class UButton;
class UAWeekInventoryComponent;
class APreviewObject;
UCLASS()
class AWEEK_API UBuildingCraftPanel : public UMainWidgetPanel
{
	GENERATED_BODY()

public:
	virtual void ActivatePanel();
	virtual void DeactivateWidget();
	void UpdateData(const FAWeekBuildingData* Data = nullptr);
	void RemoveItem();
protected:
	UPROPERTY(meta=(BindWidget))
	UCommonHierarchicalScrollBox* HierBox;
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* BuildingName;
	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* BuildingIcon;
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* BuildingText;
	UPROPERTY(meta=(BindWidget))
	USizeBox* SizeBox;
	UPROPERTY(meta=(BindWidget))
	UButton* CreateButton;

	UPROPERTY()
	TArray<UBuildCostPill*> Pills;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBuildCostPill> CostPillClass;
	UPROPERTY()
	UAWeekInventoryComponent* InventoryComponent = nullptr;
	
	UPROPERTY()
	TSubclassOf<APreviewObject> PreviewObjectClass;

	UFUNCTION()
	void CreateBuilding();

	void CreateCostPill(const FAWeekCost Cost);
	void UpdateItemCount();

	void ApplyText(FString BuildingText);
	UPROPERTY()
	bool bCheck = false;
	UPROPERTY()
	bool bInventoryCheck = true;
};
