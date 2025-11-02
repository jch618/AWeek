// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SettingWidgetMain.generated.h"

class UCommonButtonBase;
class UCommonTabListWidgetBase;
class USettingWidgetPanel;
class USettingItem;
class USettingRegistry;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FTabRegistry
{
	GENERATED_BODY()

public:
	FTabRegistry(): bContentInitVisible(false)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DevName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TabText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bContentInitVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCommonButtonBase> ButtonWidgetType;
};

UCLASS()
class SETTINGSYSTEM_API USettingWidgetMain : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void Apply();

	UFUNCTION(BlueprintCallable)
	void Cancel();

	UFUNCTION(BlueprintCallable)
	void RegisterSettingTab(const FTabRegistry& TabRegistry);

	UFUNCTION(BlueprintCallable)
	void SetPanelSetting(FName DevName);

	UFUNCTION(BlueprintImplementableEvent)
	USettingRegistry* CreateRegistry();

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeDirtyState(bool bSettingDirty);

	void HandleSettingChanged(USettingItem* Setting);

private:
	UPROPERTY(Transient)
	TObjectPtr<USettingRegistry> Registry;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USettingWidgetPanel> Panel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTabListWidgetBase> TabList;
};
