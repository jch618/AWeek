// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/MainWidget/MainUIWidget.h"
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


void UMainUIWidget::NativeOnDeactivated()
{
	UE_LOG(LogTemp, Log, TEXT("MainUIWidget deactivated test"));

	if (PanelSwitcher)
	{
		PanelSwitcher->SetActiveWidgetIndex(0);
	}
	Super::NativeOnDeactivated();
}





