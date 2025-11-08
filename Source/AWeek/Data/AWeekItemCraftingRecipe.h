// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Data/AWeekItemDataStructs.h"
#include "AWeekItemCraftingRecipe.generated.h"


/**
 * 
 */
USTRUCT()
struct FAWeekRequiredIngredientItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FDataTableRowHandle ItemHandle;

	UPROPERTY(EditAnywhere)
	int32 RequiredQuantity;
};

USTRUCT()
struct FAWeekItemCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

	// What the recipe produces
	UPROPERTY(EditAnywhere, Category = "Recipe Output")
	FDataTableRowHandle CraftedItem; // Use a Data Table Row Handle

	UPROPERTY(EditAnywhere, Category = "Recipe Output")
	int32 CraftedAmount;

	UPROPERTY(EditAnywhere, Category = "Recipe Ingredients")
	//FAWeekIngredientItemList IngredientItemList;
	TArray<FAWeekRequiredIngredientItem> IngredientItems;
};

USTRUCT()
struct FAWeekCachedCraftingRecipe
{
	GENERATED_BODY()
	
	UPROPERTY()
	FAWeekItemCraftingRecipe OriginalRecipe;

	UPROPERTY()
	FAWeekItemEntry CraftedItemEntry;

	UPROPERTY()
	TArray<FAWeekItemEntry> IngredientItemEntries;

	bool bIsCacheValid = false;
};