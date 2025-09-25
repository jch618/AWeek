// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekCraftingComponent.generated.h"

struct FAWeekCachedCraftingRecipe;
class AAWeekPlayerCharacter;
struct FAWeekItemCraftingRecipe;
class UAWeekInventoryComponent;

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
	FORCEINLINE const FAWeekCachedCraftingRecipe& GetRecipeAt(int32 RecipeIndex) const;
	bool CanCraft(int32 RecipeIndex);
	bool CanCraft(const FAWeekItemCraftingRecipe& CraftingRecipe);
	bool CanCraft(const FAWeekCachedCraftingRecipe& CachedCraftingRecipe);

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
	virtual void BeginPlay() override;

	bool TryCraftItem(int32 RecipeIndex);
private:
	void CacheCraftingRecipes();
};
