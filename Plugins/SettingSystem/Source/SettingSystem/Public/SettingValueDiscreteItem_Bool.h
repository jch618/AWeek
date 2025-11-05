// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingValueDiscreteItem.h"
#include "SettingValueDiscreteItem_Bool.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingValueDiscreteItem_Bool : public USettingValueDiscreteItem
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	void SetDefaultValue(bool InValue);
};
