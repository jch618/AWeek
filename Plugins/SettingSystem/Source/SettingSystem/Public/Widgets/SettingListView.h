// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "SettingListView.generated.h"

class USettingWidgetTypeData;
/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingListView : public UListView
{
	GENERATED_BODY()

	virtual TSubclassOf<UUserWidget> GetDesiredEntryClassForItem(UObject* Item) const override;
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USettingWidgetTypeData> WidgetTypeData;
};
