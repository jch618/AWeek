// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SettingListViewItem.generated.h"

class USettingItem;
/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingListViewItem : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void Init(USettingItem* InGameSetting);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleInit();
protected:
	UPROPERTY(Transient)
	TObjectPtr<USettingItem> Setting;

	UPROPERTY(BlueprintReadOnly)
	FText DisplayText;

protected:
	void HandleSettingChangedApplied(USettingItem* ChangedSetting);
};
