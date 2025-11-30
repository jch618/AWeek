// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SettingListViewItem.generated.h"

enum class ESettingChangedReason;
class UTextBlock;
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
	virtual void NativeOnEntryReleased() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void HandleInit();
protected:
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetDesiredGamepadWidget();
	
	UPROPERTY(Transient)
	TObjectPtr<USettingItem> Setting;

	UPROPERTY(BlueprintReadOnly)
	FText DisplayText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> SettingTextBlock; 
protected:
	virtual void HandleSettingChangedApplied(USettingItem* ChangedSetting, ESettingChangedReason Reason);
};
