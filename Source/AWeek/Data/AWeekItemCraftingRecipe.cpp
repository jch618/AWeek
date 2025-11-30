// AWeekItemCraftingRecipe.cpp
#include "AWeek/Data/AWeekItemCraftingRecipe.h"

FAWeekItemEntry FAWeekCraftingRecipeParser::ParseItemString(const FString& ItemString, UDataTable* ItemDataTable)
{
    if (ItemString.IsEmpty() || !ItemDataTable)
    {
        return FAWeekItemEntry();
    }

    // "Sword:1" → ["Sword", "1"]
    TArray<FString> Parts;
    ItemString.ParseIntoArray(Parts, TEXT(":"), true);

    if (Parts.Num() != 2)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid item string format: %s. Expected 'ItemID:Quantity'"), *ItemString);
        return FAWeekItemEntry();
    }

    FName ItemID = FName(*Parts[0].TrimStartAndEnd());
    int32 Quantity = FCString::Atoi(*Parts[1].TrimStartAndEnd());
    
    const FAWeekItemData* ItemData = ItemDataTable->FindRow<FAWeekItemData>(ItemID, TEXT(""));
    if (!ItemData)
    {
        UE_LOG(LogTemp, Error, TEXT("Item not found in DataTable: %s"), *ItemID.ToString());
        return FAWeekItemEntry();
    }

    return FAWeekItemEntry(*ItemData, Quantity);
}

TArray<FAWeekItemEntry> FAWeekCraftingRecipeParser::ParseIngredientsString(
    const FString& IngredientsString, 
    UDataTable* ItemDataTable)
{
    TArray<FAWeekItemEntry> Ingredients;

    if (IngredientsString.IsEmpty() || !ItemDataTable)
    {
        return Ingredients;
    }

    // "Wood:5,Iron:3,Leather:2" → ["Wood:5", "Iron:3", "Leather:2"]
    TArray<FString> IngredientStrings;
    IngredientsString.ParseIntoArray(IngredientStrings, TEXT(","), true);

    for (const FString& IngredientString : IngredientStrings)
    {
        FAWeekItemEntry Ingredient = ParseItemString(IngredientString, ItemDataTable);
        if (Ingredient.ItemData.ID != NAME_None)
        {
            Ingredients.Add(Ingredient);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s is invalid name"), *Ingredient.ItemData.ID.ToString());
        }
    }

    return Ingredients;
}

void FAWeekCraftingRecipeParser::ConvertRecipe(const FAWeekItemCraftingRecipeCSV& CSVRecipe,
    FAWeekItemEntry& OutCraftedItem,
    TArray<FAWeekItemEntry>& OutIngredients,
    UDataTable* ItemDataTable)
{
    OutCraftedItem = ParseItemString(CSVRecipe.CraftedItem, ItemDataTable);
    OutIngredients = ParseIngredientsString(CSVRecipe.IngredientItems, ItemDataTable);
}