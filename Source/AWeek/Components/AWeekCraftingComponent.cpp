
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
	LoadCraftingRecipeData();
	CacheCraftingRecipes();
	
	PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwner());
	InventoryComponent = PlayerCharacter->GetPlayerInventoryComponent();
}

void UAWeekCraftingComponent::LoadCraftingRecipeData()
{
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
}

bool UAWeekCraftingComponent::TryCraftRecipe(int32 RecipeIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (CachedCraftingRecipes.IsValidIndex(RecipeIndex))
	{
		const FAWeekCachedCraftingRecipe& Recipe = CachedCraftingRecipes[RecipeIndex];
		if (!InventoryComponent->CanAddItem(Recipe.CraftedItemEntry.ItemData.ID,
			Recipe.CraftedItemEntry.ItemData.NumericData.Weight,
			Recipe.CraftedItemEntry.Quantity))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Can't Craft Item because there is no space in inventory."),
				*FString(__FUNCTION__));
			return false;
		}
		if (TryConsumeIngredients(Recipe.IngredientItemEntries))
		{
			UAWeekItemBase* CraftedItem = CreateCraftedItem(Recipe.CraftedItemEntry);
			InventoryComponent->HandleAddItem(CraftedItem);
			return true;
		}
	}
	return false;
}

bool UAWeekCraftingComponent::TryConsumeIngredients(const TArray<FAWeekItemEntry>& IngredientItemEntries)
{
	for (const FAWeekItemEntry& IngredientItemEntry : IngredientItemEntries)
	{
		if (InventoryComponent->TryRemoveAmountOfItem(IngredientItemEntry.ItemData.ID, IngredientItemEntry.Quantity))
		{
			if (InventoryItemCounts.Contains(IngredientItemEntry.ItemData.ID))
			{
				InventoryItemCounts[IngredientItemEntry.ItemData.ID] -= IngredientItemEntry.Quantity;
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

UAWeekItemBase* UAWeekCraftingComponent::CreateCraftedItem(const FAWeekItemEntry& CraftedItemEntry)
{
	UAWeekItemBase* ResultItem = NewObject<UAWeekItemBase>(this, UAWeekItemBase::StaticClass());
	ResultItem->InitializeItem(CraftedItemEntry.ItemData, CraftedItemEntry.Quantity);
	return ResultItem;
}

void UAWeekCraftingComponent::CacheCraftingRecipes()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (IsValid(CraftingRecipesTable))
	{
		// Access all the rows in the data table
		TArray<FAWeekItemCraftingRecipe*> CraftingRecipePointers;
		CraftingRecipesTable->GetAllRows<FAWeekItemCraftingRecipe>(FString(), CraftingRecipePointers);

		for (const FAWeekItemCraftingRecipe* Recipe : CraftingRecipePointers)
		{
			if (Recipe)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s: Cache recipe"), *FString(__FUNCTION__));
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

void UAWeekCraftingComponent::UpdateInventoryCounts()
{
	if (InventoryComponent)
	{
		InventoryItemCounts = InventoryComponent->GetInventoryItemCounts();
	}
}

bool UAWeekCraftingComponent::GetRecipeAt(int32 RecipeIndex, FAWeekCachedCraftingRecipe& Recipe) const
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Clicked RecipeIndex=%d"), *FString(__FUNCTION__), RecipeIndex);
	if (CachedCraftingRecipes.IsValidIndex(RecipeIndex))
	{
		Recipe = CachedCraftingRecipes[RecipeIndex];
		return true;
	}
	return false;
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
