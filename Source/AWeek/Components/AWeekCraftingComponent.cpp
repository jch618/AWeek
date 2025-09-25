
#include "AWeek/Components/AWeekCraftingComponent.h"

#include "AWeekInventoryComponent.h"
#include "IMediaCache.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Data/AWeekItemCraftingRecipe.h"
#include "AWeek/Items/AWeekItemBase.h"

UAWeekCraftingComponent::UAWeekCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;	
}

void UAWeekCraftingComponent::InitializeCraftingComponent()
{
	CacheCraftingRecipes();
}

void UAWeekCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (IsValid(CraftingRecipesTable))
	{
		// Access all the rows in the data table
		TArray<FAWeekItemCraftingRecipe*> CraftingRecipePointers;
		CraftingRecipesTable->GetAllRows<FAWeekItemCraftingRecipe>(FString(), CraftingRecipePointers);

		for (const FAWeekItemCraftingRecipe* Recipe : CraftingRecipePointers)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Recipe add"), *FString(__FUNCTION__));
			if (Recipe)
			{
				CraftingRecipes.Add(*Recipe);
			}
		}
	}

	PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwner());
	InventoryComponent = PlayerCharacter->GetInventory();
}

bool UAWeekCraftingComponent::TryCraftItem(int32 RecipeIndex)
{
	// TODO: if fail to craft item, restore InventoryItemCounts
	const FAWeekItemCraftingRecipe& CraftingRecipe = CraftingRecipes[RecipeIndex];
	for (const FAWeekRequiredIngredientItem& IngredientItem : CraftingRecipe.IngredientItems)
	{
		const FAWeekItemData* IngredientItemData = IngredientItem.ItemHandle.GetRow<FAWeekItemData>(IngredientItem.ItemHandle.RowName.ToString());
		if (IngredientItemData)
		{
			if (InventoryComponent->TryRemoveAmountOfItem(IngredientItemData->ID, IngredientItem.RequiredQuantity))
			{
				InventoryItemCounts[IngredientItemData->ID] -= IngredientItem.RequiredQuantity;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

void UAWeekCraftingComponent::CacheCraftingRecipes()
{
	if (IsValid(CraftingRecipesTable))
	{
		// Access all the rows in the data table
		TArray<FAWeekItemCraftingRecipe*> CraftingRecipePointers;
		CraftingRecipesTable->GetAllRows<FAWeekItemCraftingRecipe>(FString(), CraftingRecipePointers);

		for (const FAWeekItemCraftingRecipe* Recipe : CraftingRecipePointers)
		{
			if (Recipe)
			{
				FAWeekCachedCraftingRecipe CachedCraftingRecipe;
				
				const FAWeekItemData* CraftedItemData = Recipe->CraftedItem.GetRow<FAWeekItemData>(Recipe->CraftedItem.RowName.ToString());
				FAWeekItemEntry CraftedItemEntry(*CraftedItemData, Recipe->CraftedAmount);
				
				TArray<FAWeekItemEntry> IngredientItemEntries;
				for (const FAWeekRequiredIngredientItem& IngredientItem : Recipe->IngredientItems)
				{
					const FAWeekItemData* IngredientItemData = IngredientItem.ItemHandle.GetRow<FAWeekItemData>(IngredientItem.ItemHandle.RowName.ToString());
					IngredientItemEntries.Add(FAWeekItemEntry(*IngredientItemData, IngredientItem.RequiredQuantity));
				}
				
				CachedCraftingRecipe.OriginalRecipe = *Recipe;
				CachedCraftingRecipe.CraftedItemEntry = CraftedItemEntry;
				CachedCraftingRecipe.IngredientItemEntries = IngredientItemEntries;
				CachedCraftingRecipe.bIsCacheValid = true;

				CachedCraftingRecipes.Add(CachedCraftingRecipe);
			}
		}
	}
}

bool UAWeekCraftingComponent::CanCraft(const FAWeekItemCraftingRecipe& CraftingRecipe)
{
	for (const FAWeekRequiredIngredientItem& IngredientItem : CraftingRecipe.IngredientItems)
	{
		const FAWeekItemData* ItemData = IngredientItem.ItemHandle.GetRow<FAWeekItemData>(IngredientItem.ItemHandle.RowName.ToString());
		if (ItemData)
		{
			if (!InventoryItemCounts.Contains(ItemData->ID) || InventoryItemCounts[ItemData->ID] < IngredientItem.RequiredQuantity)
			{
				return false;
			}
		}
	}
	return true;
}

bool UAWeekCraftingComponent::CanCraft(const FAWeekCachedCraftingRecipe& CachedCraftingRecipe)
{
	for (const FAWeekItemEntry IngredientItemEntry : CachedCraftingRecipe.IngredientItemEntries)
	{
		if (!InventoryItemCounts.Contains(IngredientItemEntry.ItemData.ID) ||
			InventoryItemCounts[IngredientItemEntry.ItemData.ID] < IngredientItemEntry.Quantity)
		{
			return false;
		}
	}
	return true;
}

const FAWeekCachedCraftingRecipe& UAWeekCraftingComponent::GetRecipeAt(int32 RecipeIndex) const
{
	return CachedCraftingRecipes[RecipeIndex];
}

bool UAWeekCraftingComponent::CanCraft(int32 RecipeIndex)
{
	if (CraftingRecipes.IsValidIndex(RecipeIndex))
	{
		return CanCraft(CraftingRecipes[RecipeIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: RecipeIndex is not valid!"), *FString(__FUNCTION__));
	}
	return false;
}
