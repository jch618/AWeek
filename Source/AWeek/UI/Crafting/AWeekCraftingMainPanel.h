// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekCraftingMainPanel.generated.h"

class UAWeekCraftingController;
class UAWeekInventoryComponent;
class UAWeekCraftingComponent;
class UAWeekCraftingDetailPanel;
class UAWeekCraftingListPanel;
class UAWeekInventoryPanel;
class AAWeekPlayerCharacter;

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekCraftingMainPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	void ShowDetailPanel();

	void InitializeCraftingMainPanel(TObjectPtr<UAWeekCraftingController> InCraftingController, const TObjectPtr<UAWeekInventoryComponent> InInventoryComponent);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekCraftingListPanel> CraftingListPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekCraftingDetailPanel> CraftingDetailPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekInventoryPanel> PlayerInventoryPanel;

	UPROPERTY()
	TObjectPtr<UAWeekCraftingController> CraftingController;
	
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnRecipeSelected(int32 RecipeIndex, bool bIsCraftable);

	UFUNCTION()
	void OnCraftButtonLeftClicked(int32 RecipeIndex);
};
