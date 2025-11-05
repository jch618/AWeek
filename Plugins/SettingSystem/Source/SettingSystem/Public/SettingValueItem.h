// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingItem.h"
#include "SettingValueItem.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingValueItem : public USettingItem
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	virtual void Reset() PURE_VIRTUAL(USettingValueItem::Reset,); // 공장 초기화
	virtual void Store() PURE_VIRTUAL(USettingValueItem::Stroe,); // 값을 저장 Apply
	virtual void Restore() PURE_VIRTUAL(USettingValueItem::Restore,); // 아직 Store 안된 값을 원래 상태로 되돌림
};
