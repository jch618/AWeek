// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekCraftingMainPanel.generated.h"

class UAWeekCraftingComponent;
class UAWeekCraftingDetailPanel;
class UAWeekCraftingListPanel;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekCraftingMainPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	void ShowDetailPanel();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekCraftingListPanel> CraftingListPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekCraftingDetailPanel> CraftingDetailPanel;

	UPROPERTY()
	TObjectPtr<UAWeekCraftingComponent> CraftingComponent;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnRecipeSelected(int32 RecipeIndex);
};
