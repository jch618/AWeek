// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekPlayerHotBar.generated.h"

struct FAWeekInventorySlotData;
class UAWeekPlayerInventoryComponent;
class UUniformGridPanel;
class UAWeekItemSlot;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekPlayerHotBar : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	void InitializeHotBar(TObjectPtr<UAWeekPlayerInventoryComponent> InPlayerInventoryComponent);

	UFUNCTION()
	void RefreshHotBar();

	UFUNCTION()
	void OnHotBarSelectionChanged(int32 OldIndex, int32 NewIndex);
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAWeekItemSlot> ItemSlotClass; 

	UPROPERTY()
	TObjectPtr<UAWeekPlayerInventoryComponent> PlayerInventoryComponent;

	UFUNCTION()
	void OnSlotUpdated(const FAWeekInventorySlotData& SlotData);
private:
	UPROPERTY()
	TArray<TObjectPtr<UAWeekItemSlot>> HotBarSlots;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> HotBarGridPanel;
};
