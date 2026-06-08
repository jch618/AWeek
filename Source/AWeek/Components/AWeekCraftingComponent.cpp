
#include "AWeek/Components/AWeekCraftingComponent.h"

#include "AWeekInventoryComponent.h"
#include "AWeekPlayerInventoryComponent.h"
#include "IMediaCache.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Data/AWeekItemCraftingRecipe.h"
#include "AWeek/Items/AWeekItemBase.h"

UAWeekCraftingComponent::UAWeekCraftingComponent() : CurrentCraftingLevel(0)
{
	PrimaryComponentTick.bCanEverTick = false;	
}

void UAWeekCraftingComponent::InitializeCraftingComponent()
{
	LoadAndCacheRecipes();

	PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwner());
	PlayerInventoryComponent = PlayerCharacter->GetPlayerInventoryComponent();

	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	OnCraftingFinished.AddUObject(this, &UAWeekCraftingComponent::UpdateInventoryCounts);
}

bool UAWeekCraftingComponent::TryCraftRecipe(int32 RecipeIndex)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (CachedCraftingRecipes.IsValidIndex(RecipeIndex))
	{
		const FAWeekCachedCraftingRecipe& Recipe = CachedCraftingRecipes[RecipeIndex];
		if (!PlayerInventoryComponent->CanAddItem(Recipe.CraftedItemEntry.ItemData.ID,
			Recipe.CraftedItemEntry.ItemData.NumericData.Weight,
			Recipe.CraftedItemEntry.Quantity))
		{
			return false;
		}
		if (TryConsumeIngredients(Recipe.IngredientItemEntries))
		{
			UAWeekItemBase* CraftedItem = CreateCraftedItem(Recipe.CraftedItemEntry);
			PlayerInventoryComponent->HandleAddItem(CraftedItem);
			OnCraftingFinished.Broadcast();
			return true;
		}
	}
	return false;
}

bool UAWeekCraftingComponent::TryConsumeIngredients(const TArray<FAWeekItemEntry>& IngredientItemEntries)
{
	for (const FAWeekItemEntry& IngredientItemEntry : IngredientItemEntries)
	{
		if (!PlayerInventoryComponent->TryRemoveAmountOfItem(IngredientItemEntry.ItemData.ID, IngredientItemEntry.Quantity))
		{
			return false;
		}
	}
	return true;
}

UAWeekItemBase* UAWeekCraftingComponent::CreateCraftedItem(const FAWeekItemEntry& CraftedItemEntry)
{
	return UAWeekItemBase::CreateFromData(CraftedItemEntry.ItemData, CraftedItemEntry.Quantity, this);
}

void UAWeekCraftingComponent::LoadAndCacheRecipes()
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString(__FUNCTION__));
	CachedCraftingRecipes.Empty();

	if (!CraftingRecipeTable || !ItemDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("CraftingRecipeTable or ItemDataTable is null!"));
		return;
	}

	TArray<FAWeekItemCraftingRecipeCSV*> AllRecipes;
	CraftingRecipeTable->GetAllRows<FAWeekItemCraftingRecipeCSV>(TEXT(""), AllRecipes);

	for (const FAWeekItemCraftingRecipeCSV* CSVRecipe : AllRecipes)
	{
		if (!CSVRecipe)
		{
			UE_LOG(LogTemp, Error, TEXT("Recipe is null"));
			continue;
		}

		FAWeekCachedCraftingRecipe CachedRecipe;
		
		FAWeekCraftingRecipeParser::ConvertRecipe(
			*CSVRecipe,
			CachedRecipe.CraftedItemEntry,
			CachedRecipe.IngredientItemEntries,
			ItemDataTable
		);
		
		CachedRecipe.RequiredCraftingLevel = CSVRecipe->RequiredCraftingLevel;
		CachedRecipe.bIsCacheValid = true;

		CachedCraftingRecipes.Add(CachedRecipe);
	}

	UE_LOG(LogTemp, Log, TEXT("Loaded %d crafting recipes"), CachedCraftingRecipes.Num());

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

void UAWeekCraftingComponent::SetCraftingLevel(int32 InCraftingLevel)
{
	if (InCraftingLevel != CurrentCraftingLevel)
	{
		CurrentCraftingLevel = InCraftingLevel;
		OnCraftingLevelChanged.Broadcast();
	}
}

bool UAWeekCraftingComponent::CanCraft(int32 RecipeIndex)
{
	if (CachedCraftingRecipes.IsValidIndex(RecipeIndex))
	{
		return CanCraft(CachedCraftingRecipes[RecipeIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: RecipeIndex is not valid!"), *FString(__FUNCTION__));
	}
	return false;
}

void UAWeekCraftingComponent::UpdateInventoryCounts()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (PlayerInventoryComponent)
	{
		InventoryItemCounts = PlayerInventoryComponent->GetInventoryItemCounts();
	}
}

bool UAWeekCraftingComponent::GetRecipeAt(int32 RecipeIndex, FAWeekCachedCraftingRecipe& Recipe) const
{
	if (CachedCraftingRecipes.IsValidIndex(RecipeIndex))
	{
		Recipe = CachedCraftingRecipes[RecipeIndex];
		return true;
	}
	return false;
}

TArray<int32> UAWeekCraftingComponent::GetAvailableRecipes() const
{
	TArray<int32> AvailableIndices;
	for (int32 i = 0; i < CachedCraftingRecipes.Num(); i++)
	{
		if (CachedCraftingRecipes[i].RequiredCraftingLevel <= CurrentCraftingLevel)
		{
			AvailableIndices.Add(i);
		}
	}
	return AvailableIndices;
}
