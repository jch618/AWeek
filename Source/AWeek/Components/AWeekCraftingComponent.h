// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekCraftingComponent.generated.h"

struct FAWeekItemEntry;
struct FAWeekCachedCraftingRecipe;
class AAWeekPlayerCharacter;
struct FAWeekItemCraftingRecipe;
class UAWeekInventoryComponent;
class UAWeekItemBase;

DECLARE_MULTICAST_DELEGATE(FOnCraftingComponentUpdated)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UAWeekCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	FOnCraftingComponentUpdated OnCraftingComponentUpdated;

	//================================================================
	//	FUNCTIONS
	//================================================================
	UAWeekCraftingComponent();

	void InitializeCraftingComponent();

	FORCEINLINE const TArray<FAWeekCachedCraftingRecipe>& GetCachedCraftingRecipes() const { return CachedCraftingRecipes; } 
	FORCEINLINE bool GetRecipeAt(int32 RecipeIndex, FAWeekCachedCraftingRecipe& Recipe) const;

	bool TryCraftRecipe(int32 RecipeIndex);
	
	bool CanCraft(int32 RecipeIndex);
	bool CanCraft(const FAWeekItemCraftingRecipe& CraftingRecipe);
	bool CanCraft(const FAWeekCachedCraftingRecipe& CachedCraftingRecipe);

	void UpdateInventoryCounts();
protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY(EditAnywhere, Category = "Crafting")
	TObjectPtr<UDataTable> CraftingRecipesTable;

	UPROPERTY(VisibleAnywhere, Category = "Crafting")
	TArray<FAWeekItemCraftingRecipe> CraftingRecipes;

	UPROPERTY(VisibleAnywhere, Category = "Crafting")
	TArray<FAWeekCachedCraftingRecipe> CachedCraftingRecipes;
	
	// (ItemID, Count)
	UPROPERTY(VisibleAnywhere, Category = "Crafting")
	TMap<FName, int32> InventoryItemCounts;
	
	UPROPERTY(VisibleAnywhere, Category = "Crafting")
	TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Crafting")
	TObjectPtr<UAWeekInventoryComponent> InventoryComponent;
	//================================================================
	//	FUNCTIONS
	//================================================================

	bool TryConsumeIngredients(const TArray<FAWeekItemEntry>& IngredientItemEntries);
	UAWeekItemBase* CreateCraftedItem(const FAWeekItemEntry& CraftedItemEntry);
private:
	void CacheCraftingRecipes();
	void LoadCraftingRecipeData();
};
