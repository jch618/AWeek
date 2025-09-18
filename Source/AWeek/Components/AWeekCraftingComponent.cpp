
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "AWeek/Data/AWeekItemCraftingRecipe.h"

UAWeekCraftingComponent::UAWeekCraftingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	
}


void UAWeekCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CraftingRecipesTable))
	{
		// Access all the rows in the data table
		TArray<FAWeekItemCraftingRecipe*> CraftingRecipePointers;
		CraftingRecipesTable->GetAllRows<FAWeekItemCraftingRecipe>(FString(), CraftingRecipePointers);

		for (const FAWeekItemCraftingRecipe* Recipe : CraftingRecipePointers)
		{
			if (Recipe)
			{
				CraftingRecipes.Add(*Recipe);
			}
		}
	}
}
