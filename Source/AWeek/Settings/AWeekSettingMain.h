// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SettingWidgetMain.h"
#include "AWeekSettingMain.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekSettingMain : public USettingWidgetMain
{
	GENERATED_BODY()

	virtual USettingRegistry* CreateRegistry() override;
};
