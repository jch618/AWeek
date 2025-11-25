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
	
	UPROPERTY(EditAnywhere, Category = "Crafting Recipe")
	FDataTableRowHandle CraftedItem; 

	UPROPERTY(EditAnywhere, Category = "Crafting Recipe")
	int32 CraftedAmount;

	UPROPERTY(EditAnywhere, Category = "Crafting Recipe")
	TArray<FAWeekRequiredIngredientItem> IngredientItems;

	UPROPERTY(EditAnywhere, Category = "Crafting Recipe")
	int32 CraftingLevel;
};

// USTRUCT()
// struct FAWeekItemCraftingRecipeCSV
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditAnywhere, Category = "Crafting Recipe")
// 	FString CraftedItem;
// };

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

	UPROPERTY()
	int32 CraftingLevel;
	
	bool bIsCacheValid = false;
};