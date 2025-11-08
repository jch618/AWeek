// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekCraftingDetailPanel.generated.h"

class UButton;
class UHorizontalBox;
class UAWeekItemSlot;
class UAWeekCraftingComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCraftButtonClicked, int32 CurrentRecipeIndex)

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekCraftingDetailPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	FOnCraftButtonClicked OnCraftButtonLeftClicked;

	void InitializeCraftingDetailPanel();
	void SetRecipe(int32 RecipeIndex, bool bIsCraftable);

	UFUNCTION()
	void UpdateCraftButton();
	
	void ClearCraftingDetails();
	FORCEINLINE int32 GetCurrentRecipeIndex() const { return CurrentRecipeIndex; }
	FORCEINLINE void SetCraftingComponent(TObjectPtr<UAWeekCraftingComponent> InCraftingComponent) { CraftingComponent = InCraftingComponent; }
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAWeekItemSlot> ItemSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekItemSlot> ResultItemSlot;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> IngredientsContainer; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CraftButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAWeekItemSlot> IngredientSlotClass;

	// UPROPERTY()
	// TObjectPtr<UAWeekCraftingComponent> CraftingComponent;

	virtual void NativeConstruct() override;
	UPROPERTY()
	int32 CurrentRecipeIndex = -1;

private:
	void UpdateIngredientSlots();

protected:

private:
	UPROPERTY()
	TObjectPtr<UAWeekCraftingComponent> CraftingComponent;
	
	UFUNCTION()
	void OnCraftButtonClicked();
};
