// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/MainWidget/Panel/BuildingCraftPanel.h"

#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "AWeek/Data/AWeekBuildingData.h"
#include "AWeek/Data/AWeekItemDataStructs.h"
#include "AWeek/UI/MainWidget/BuildCostPill.h"
#include "Compilation/MovieSceneCompiledDataManager.h"
#include "CommonHierarchicalScrollBox.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "Components/Button.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "Components/SizeBox.h"

void UBuildingCraftPanel::ActivatePanel()
{
	//Nothing
	//UE_LOG(LogTemp, Log, TEXT("Crafting Panel Active"));
	if (CreateButton)
	{
		CreateButton->OnClicked.Clear();
		CreateButton->OnClicked.AddDynamic(this, &UBuildingCraftPanel::CreateBuilding);
	}
}

void UBuildingCraftPanel::UpdateData(const FAWeekBuildingData* Data)
{
	if (bCheck){UpdateItemCount(); return;}
	if (Data == nullptr){UpdateItemCount();}
	BuildingName->SetText(FText::FromName(Data->ID));
	BuildingIcon->SetBrushFromTexture(Data->Image);
	BuildingText->SetText(FText::FromString(Data->BuildingText));
	
	bCheck = true;
	for (FAWeekCost Cost : Data->Costs)
	{
		CreateCostPill(Cost);
	}
	UpdateItemCount();
}

void UBuildingCraftPanel::UpdateItemCount()
{
	UE_LOG(LogTemp, Log, TEXT("BuildingCraftPanel::UpdateItemCount"));
	bInventoryCheck = true;
	for (UBuildCostPill* Pill : Pills)
	{
		if (Pill->CheckItem())bInventoryCheck = false;
	}
	if (bInventoryCheck){CreateButton->SetIsEnabled(false);}
	else{CreateButton->SetIsEnabled(true);}
}

void UBuildingCraftPanel::DeactivateWidget()
{
	Super::DeactivateWidget();
	UE_LOG(LogTemp, Error, TEXT("BuildingCraftPanel DeactivateWidget!"));
}

void UBuildingCraftPanel::CreateCostPill(const FAWeekCost Cost)
{
	if (CostPillClass == nullptr){return;}
	UBuildCostPill* Pill = CreateWidget<UBuildCostPill>(this, CostPillClass);
	if (!Cost.ItemRow.DataTable || !Cost.ItemRow.RowName.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cost.ItemRow not set"));
		return;
	}
	const FString Ctx = TEXT("GetItemDataFromCost");
	UE_LOG(LogTemp, Warning, TEXT("Test1"));
	if (const FAWeekItemData* Row = Cost.ItemRow.GetRow<FAWeekItemData>(Ctx))
	{
		Pill->SetImage(Row->AssetData.Icon);
		Pill->SetName(Row->TextData.Name);
		Pill->SetItemID(Row->ID);
		UE_LOG(LogTemp, Warning, TEXT("Test2"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Test3"));
	Pill->SetCount(Cost.Amount);
	HierBox->AddChild(Pill);
	Pills.Add(Pill);

	if (InventoryComponent == nullptr)
	{
		if (APlayerController* PC = GetOwningPlayer())
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				if (AAWeekPlayerCharacter* Ch = Cast<AAWeekPlayerCharacter>(Pawn))
				{
					InventoryComponent = Ch->FindComponentByClass<UAWeekInventoryComponent>();
				}
			}
		}
	}
	Pill->InventoryComponent = InventoryComponent;
	
}

void UBuildingCraftPanel::CreateBuilding()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Building!!"));
	//TODO Grid setting
	//TODO Remove Cost
}




