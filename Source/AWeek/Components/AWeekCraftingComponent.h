// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekCraftingComponent.generated.h"

struct FAWeekItemCraftingRecipe;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UAWeekCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	

	//================================================================
	//	FUNCTIONS
	//================================================================
	UAWeekCraftingComponent();

protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY(EditInstanceOnly, Category = "Crafting")
	TObjectPtr<UDataTable> CraftingRecipesTable;

	UPROPERTY(VisibleAnywhere, Category = "Crafting")
	TArray<FAWeekItemCraftingRecipe> CraftingRecipes;
	//================================================================
	//	FUNCTIONS
	//================================================================
	virtual void BeginPlay() override;		
};
