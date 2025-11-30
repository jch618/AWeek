// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Building/BuildingSelectNotionPanel.h"

#include "CommonHierarchicalScrollBox.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Data/AWeekBuildingData.h"
#include "AWeek/UI/MainWidget/BuildCostPill.h"

void UBuildingSelectNotionPanel::UpdateData(const FAWeekBuildingData* Data)
{
	//ClearData();
	if (Data == nullptr)return;
	if (!bCreateCheck)
	{
		BuildingName->SetText(FText::FromName(Data->ID));
		BuildingIcon->SetBrushFromTexture(Data->Image);
		//BuildingIcon->SetBrushFromMaterial(Data->IconMaterial);
		//BuildingText->SetText(FText::FromString(Data->BuildingText));
		ApplyText(Data->BuildingText);
	
	
		bCheck = true;
		for (FAWeekCost Cost : Data->Costs)
		{
			CreateCostPill(Cost);
		}
		bCreateCheck = true;
	}
	
	
	UpdateItemCount();
	if (bTrue)
	{
		UE_LOG(LogTemp, Log, TEXT("true"));
		
	}else{UE_LOG(LogTemp, Log, TEXT("False"));}
}

void UBuildingSelectNotionPanel::ClearData()
{
	HierBox->ClearChildren();
}


void UBuildingSelectNotionPanel::RemoveItem()
{
	for (UBuildCostPill* Pill : Pills)
	{
		if (!Pill->RemoveItem())
		{
			UE_LOG(LogTemp, Error, TEXT("Item Lack!! This Error must be solved"));
		}
	}
}

void UBuildingSelectNotionPanel::UpdateItemCount()
{
	bCheck = true;
	for (UBuildCostPill* Pill : Pills)
	{
		UE_LOG(LogTemp, Log, TEXT("asddasdfsa"));
		if (Pill->CheckItem())bTrue = false;
	}
	
}

void UBuildingSelectNotionPanel::ApplyText(FString BuildingText_)
{
	if (!BuildingText) return;

	FString S = BuildingText_;         
	/*S.ReplaceInline(TEXT("\\r\\n"), TEXT("\n"));
	S.ReplaceInline(TEXT("\\n"), TEXT("\n"));*/

	S.ReplaceInline(TEXT("\\r\\n"), TEXT("\n"));
	S.ReplaceInline(TEXT("\\n"), TEXT("\n"));


	S.ReplaceInline(TEXT(" \n"), TEXT("\n"));
	S.ReplaceInline(TEXT("\n "), TEXT("\n"));
	

	BuildingText->SetText(FText::FromString(S));	
}

void UBuildingSelectNotionPanel::CreateCostPill(const FAWeekCost Cost)
{
	if (CostPillClass == nullptr){return;}
	UBuildCostPill* Pill = CreateWidget<UBuildCostPill>(this, CostPillClass);
	if (!Cost.ItemRow.DataTable || !Cost.ItemRow.RowName.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cost.ItemRow not set"));
		return;
	}
	const FString Ctx = TEXT("GetItemDataFromCost");
	if (const FAWeekItemData* Row = Cost.ItemRow.GetRow<FAWeekItemData>(Ctx))
	{
		Pill->SetImage(Row->AssetData.Icon);
		Pill->SetName(Row->TextData.Name);
		Pill->SetItemID(Row->ID);
	}
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



