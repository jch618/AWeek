// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekInventoryPanel.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekPlayerInventoryPanel.generated.h"


class UAWeekPlayerInventoryComponent;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekPlayerInventoryPanel : public UAWeekInventoryPanel
{
	GENERATED_BODY()

public:
	void LinkToInventory(TObjectPtr<UAWeekPlayerInventoryComponent> InInventoryComponent);
	
protected:
	UPROPERTY()
	TObjectPtr<UAWeekPlayerInventoryComponent> PlayerInventoryComponent;

	virtual void InitializeGridPanel() override;
	virtual void RefreshInventoryPanel() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> HotBarGridPanel;	
};
