// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SettingWidgetPanel.generated.h"

class USettingListView;
class USettingItem;
/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingWidgetPanel : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void RefreshSetting(USettingItem* RootSetting);

protected:
	void AppendSettingChild(USettingItem* Setting, TArray<UObject*>& InOutSettings);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USettingListView> ListView;
};
