// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TestWidget/MainUIWidget.h"
#include "AWeek/UI/AWeekActivatableWidget.h"

void UMainUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ensure(PanelSwitcher))
	{
		PanelSwitcher->SetActiveWidgetIndex(0);
	}

	if (BtnInventory) BtnInventory->OnClicked().AddUObject(this, &ThisClass::ShowInventory);
	if (BtnBuilding) BtnBuilding->OnClicked().AddUObject(this, &ThisClass::ShowBuilding);
}


