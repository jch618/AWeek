// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/MainWidget/MainWidgetPanel.h"
#include "AWeek/Data/AWeekBuildingData.h"
#include "BuildingPanel.generated.h"

/**
 * 
 */
class UCommonHierarchicalScrollBox;
class UBuildingButton;
class UCommonActivatableWidgetSwitcher;
class UBuildingCraftPanel;

UCLASS()
class AWEEK_API UBuildingPanel : public UMainWidgetPanel
{
	GENERATED_BODY()
	
public:
	virtual void SetActive() override;

	virtual void NativeOnDeactivated() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonHierarchicalScrollBox* HierBox;
	UPROPERTY(meta=(BindWidget))
	UCommonActivatableWidgetSwitcher* PanelSwitcher;

	
	UPROPERTY(EditAnywhere, Category = "Building Data")
	UDataTable* BuildingData;

	UPROPERTY(EditAnywhere, Category = "Building Data")
	TSubclassOf<UBuildingButton> BuildingButtonClass;

	UPROPERTY(EditAnywhere, Category = "Building Data")
	TSubclassOf<UBuildingCraftPanel> BuildingCraftPanelClass;

	UPROPERTY()
	bool bFindData = false;

	void HandleBuildingButtonClicked(FName RowName, UBuildingButton* Btn);
	UBuildingCraftPanel* CreateOrGetPanel(const FName RowName, FAWeekBuildingData RowData);
};
