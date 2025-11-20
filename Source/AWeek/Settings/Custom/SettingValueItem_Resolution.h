// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingValueDiscreteItem.h"
#include "SettingValueItem_Resolution.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API USettingValueItem_Resolution : public USettingValueDiscreteItem
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	
	// 초기화, 저장 Setting 내부에서 안함
	virtual void Store() override;
	virtual void Restore() override;
	virtual void Reset() override;

	virtual int32 GetValueOptionIndex() const override;
	virtual void SetValueOptionByIndex(int32 Index) override;

protected:
	TArray<FIntPoint> Resolutions;
};
