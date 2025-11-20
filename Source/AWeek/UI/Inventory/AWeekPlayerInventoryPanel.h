// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekInventoryPanel.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekPlayerInventoryPanel.generated.h"

class UAWeekInventoryItemSlot;
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
	// virtual void BindInventoryDelegates() override;
	// virtual void UnBindInventoryDelegates() override;
	virtual void OnSlotUpdate(const FAWeekInventorySlotData& SlotData) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> HotBarGridPanel;

	UPROPERTY()
	TArray<UAWeekInventoryItemSlot*> HotBarSlots;
};
