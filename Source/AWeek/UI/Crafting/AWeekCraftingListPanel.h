// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekCraftingListPanel.generated.h"

class UAWeekCraftingComponent;
class UAWeekCraftingItemSlot;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekCraftingListPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnRecipeSelected, int32 SelectedRecipeIndex);
	FOnRecipeSelected OnRecipeSelected;

	FORCEINLINE void SetCraftingComponent(UAWeekCraftingComponent* NewCraftingComponent) { CraftingComponent = NewCraftingComponent; }
protected:
	UAWeekCraftingListPanel();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAWeekCraftingItemSlot> CraftingSlotClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel;

	UPROPERTY()
	TObjectPtr<UAWeekCraftingComponent> CraftingComponent;

	UPROPERTY(EditDefaultsOnly)
	int32 NumCols;

	virtual void NativeConstruct() override;
	void RefreshCraftingList();
};
