// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/MainWidget/Panel/BuildingPanel.h"

#include "CommonHierarchicalScrollBox.h"
#include "AWeek/UI/MainWidget/BuildingButton.h"
#include "AWeek/UI/MainWidget/Panel/BuildingCraftPanel.h"
#include "CommonActivatableWidgetSwitcher.h"


void UBuildingPanel::NativeOnDeactivated()
{
	
	if (PanelSwitcher)
	{
		PanelSwitcher->SetActiveWidgetIndex(0);
	}
	Super::NativeOnDeactivated();
}





void UBuildingPanel::SetActive()
{
	Super::SetActive(); //Nothing


	static const FString Ctx = TEXT("Read BuildingData");
	TArray<FName> RowNames = BuildingData->GetRowNames();

	
	if (!bFindData)
	{
		for (const FName& RowName : RowNames)
		{
			const FAWeekBuildingData* Row = BuildingData->FindRow<FAWeekBuildingData>(RowName, Ctx);

			if (!Row) return;
			UE_LOG(LogTemp, Log, TEXT("Row: %s"), *Row->ID.ToString());
			UBuildingButton* Btn = CreateWidget<UBuildingButton>(this, BuildingButtonClass);
			if (!Btn) return;

			if (Row->Image == nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("No Image Selected"));
			}
			if (HierBox == nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("No HierBox Selected"));
			}
			Btn->SetImage(Row->Image);
			Btn->SetName(FText::FromName(RowName));
			
			Btn->OnClicked().AddWeakLambda(this, [this, Btn, RowName]()
			{
				HandleBuildingButtonClicked(RowName, Btn);
			});
			HierBox->AddChild(Btn);
		}
		bFindData = true;
	}
	if (PanelSwitcher)
	{
		if (UWidget* Old = PanelSwitcher->GetActiveWidget())
		{
			if (auto* BuildingCraftPanel = Cast<UBuildingCraftPanel>(Old))
			{
				BuildingCraftPanel->UpdateData();
			}
		}
	}
}

void UBuildingPanel::HandleBuildingButtonClicked(FName RowName, UBuildingButton* Btn)
{
	if (!ensure(BuildingData && PanelSwitcher && BuildingCraftPanelClass))return;

	static const FString Ctx = TEXT("UBuildingPanel::HandleBuildingButtonClicked");
	const FAWeekBuildingData* Row = BuildingData->FindRow<FAWeekBuildingData>(RowName, Ctx);
	if (!Row) return;
	
	if (Btn->Panel == nullptr)
	{
		UBuildingCraftPanel* Panel = Btn->CreateOrGetPanel(Row);
		if (Panel == nullptr)UE_LOG(LogTemp, Error, TEXT("CreateOrGetPanel is broken!!"));
		PanelSwitcher->AddChild(Panel);
	}
	PanelSwitcher->SetActiveWidget(Btn->Panel);
	Btn->Panel->ActivatePanel();
	Btn->Panel->UpdateData(Row);
}







