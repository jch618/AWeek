// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeekWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekWeaponItem : public UAWeekItemBase
{
	GENERATED_BODY()

public:	
	virtual bool UsePrimary(TObjectPtr<AAWeekPlayerCharacter> Character) override;
	virtual bool UseSecondary(TObjectPtr<AAWeekPlayerCharacter> Character) override;
private:
	
};
