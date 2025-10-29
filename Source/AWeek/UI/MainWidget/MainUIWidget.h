// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "CommonButtonBase.h"
#include "AWeek/UI/MainWidget/MainWidgetPanel.h"
#include "CommonActivatableWidgetSwitcher.h"
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
	UCommonActivatableWidgetSwitcher* PanelSwitcher;


	//Widget panel Button (WBP_ButtonPanelWidget)
	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* BtnBuilding;

	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* BtnInventory;
	//////////////////////////


	//Widget panel
	UPROPERTY(meta=(BindWidget))
	UMainWidgetPanel* InventoryPanel;
	UPROPERTY(meta=(BindWidget))
	UMainWidgetPanel* BuildingPanel;
	///////////////////////////

	virtual void NativeConstruct() override;


	//Widget panel Button Clicked
	void ShowInventory() {PanelSwitcher->SetActiveWidgetIndex(0); InventoryPanel->SetActive();};
	void ShowBuilding(){PanelSwitcher->SetActiveWidgetIndex(1); BuildingPanel->SetActive();};
	//////////////////////////
	
};
