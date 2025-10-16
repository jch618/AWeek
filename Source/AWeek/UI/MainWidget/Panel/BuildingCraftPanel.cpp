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
#include "AWeek/Grid/PreviewObject.h"
#include "AWeek/Grid/GridPlacedSubsystem.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"

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
	//BuildingText->SetText(FText::FromString(Data->BuildingText));
	ApplyText(Data->BuildingText);
	PreviewObjectClass = Data->PreviewObjectClass.LoadSynchronous();
	
	bCheck = true;
	for (FAWeekCost Cost : Data->Costs)
	{
		CreateCostPill(Cost);
	}
	UpdateItemCount();
}

void UBuildingCraftPanel::UpdateItemCount()
{
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

void UBuildingCraftPanel::CreateBuilding()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Building!!"));
	//TODO Grid setting
	if (!PreviewObjectClass) return;
    
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (!PC) return;
    
        if (UGridPlacedSubsystem* Sub = GetWorld()->GetSubsystem<UGridPlacedSubsystem>())
        {
            Sub->StartPlacement(PreviewObjectClass, PC, this);
        	AAWeekPlayerCharacter* Ch = Cast<AAWeekPlayerCharacter>(PC->GetPawn());
        	Ch->ToggleMainWidget();
        }
}

void UBuildingCraftPanel::RemoveItem()
{
	for (UBuildCostPill* Pill : Pills)
	{
		if (!Pill->RemoveItem())
		{
			UE_LOG(LogTemp, Error, TEXT("Item Lack!! This Error must be solved"));
		}
	}
}

void UBuildingCraftPanel::ApplyText(FString BuildingText_)
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






