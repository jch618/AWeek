// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "AWeekSettingTabListWidgetBase.generated.h"


USTRUCT(BlueprintType)
struct FAWeekSettingTabInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommonButtonBase> ButtonType;
};
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekSettingTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Tab")
	void AddTab(const FAWeekSettingTabInfo& InTabInfo);

protected:
	virtual void HandleTabCreation_Implementation(FName TabNameID, UCommonButtonBase* TabButton) override;
	
	UPROPERTY()
	TMap<FName, FAWeekSettingTabInfo> RegisteredTabs;
};
