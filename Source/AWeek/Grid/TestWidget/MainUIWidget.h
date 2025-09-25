// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "CommonButtonBase.h"
#include "TestPanelToButton.h"
#include "Components/WidgetSwitcher.h"
#include "MainUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UMainUIWidget : public UAWeekActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* PanelSwitcher;

	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* BtnBuilding;

	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* BtnInventory;

	UPROPERTY(meta=(BindWidget))
	UTestPanelToButton* InventoryPanel;
	UPROPERTY(meta=(BindWidget))
	UTestPanelToButton* BuildingPanel;

	virtual void NativeConstruct() override;

	void ShowInventory() {PanelSwitcher->SetActiveWidgetIndex(0); InventoryPanel->SetActive();};
	void ShowBuilding(){PanelSwitcher->SetActiveWidgetIndex(1); BuildingPanel->SetActive();};
	
};
