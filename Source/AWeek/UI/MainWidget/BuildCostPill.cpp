// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/MainWidget/BuildCostPill.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "AWeek/Components/AWeekInventoryComponent.h"

bool UBuildCostPill::CheckItem()
{
	if (!InventoryComponent || !ItemCount)return false;

	
	TMap<FName, int32> InventoryItem = InventoryComponent->GetInventoryItemCounts();
	//TODO ItemCheck
	if (InventoryItem.Contains(ItemID) && InventoryItem[ItemID] >= RequiredCount)
	{
		ItemCount->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		UE_LOG(LogTemp, Warning, TEXT("InventoryItem true!!!"));
		return true;
	}else
	{
		ItemCount->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		UE_LOG(LogTemp, Warning, TEXT("InventoryItem false!!!"));
		return false;
	}
}



void UBuildCostPill::SetName(FText Name)
{
	ItemName->SetText(Name);
}
void UBuildCostPill::SetImage(UTexture2D* Image)
{
	ItemImage->SetBrushFromTexture(Image);
}
void UBuildCostPill::SetCount(int32 Count)
{
	ItemCount->SetText(FText::Format(NSLOCTEXT("UI", "QtyTimes", "×{0}"),
	FText::AsNumber(Count)));
	RequiredCount = Count;
}

void UBuildCostPill::SetItemID(FName ID)
{
	ItemID = ID;
}


