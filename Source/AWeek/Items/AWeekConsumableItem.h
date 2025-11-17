// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeekConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekConsumableItem : public UAWeekItemBase
{
	GENERATED_BODY()

public:
	virtual bool UseSecondary(TObjectPtr<AAWeekPlayerCharacter> Character) override;

private:
};
