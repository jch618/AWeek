// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SettingWidgetMain.generated.h"

enum class ESettingChangedReason;
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

	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	UFUNCTION(BlueprintCallable)
	void Apply();

	UFUNCTION(BlueprintCallable)
	void Cancel();

	UFUNCTION(BlueprintCallable)
	void SetPanelSetting(FName DevName);

	UFUNCTION(BlueprintCallable)
	USettingItem* FindByRootSettingDevName(const FName& DevName);

	virtual USettingRegistry* CreateRegistry() PURE_VIRTUAL(CreateRegistry, return nullptr;);

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeDirtyState(bool bSettingDirty);

	void HandleSettingChanged(USettingItem* Setting, ESettingChangedReason Reason);

private:
	UPROPERTY(Transient)
	TObjectPtr<USettingRegistry> Registry;

	UPROPERTY(Blueprintable, meta = (BindWidget))
	TObjectPtr<USettingWidgetPanel> Panel;
};

