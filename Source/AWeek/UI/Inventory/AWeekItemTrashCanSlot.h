// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/Inventory/AWeekItemSlot.h"
#include "AWeekItemTrashCanSlot.generated.h"

DECLARE_DELEGATE(FOnTrashCanSlotLeftClick)
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekItemTrashCanSlot : public UAWeekItemSlot
{
	GENERATED_BODY()

	FOnTrashCanSlotLeftClick OnLeftClick;
	
	
};
