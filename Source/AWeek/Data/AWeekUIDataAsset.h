// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AWeekUIDataAsset.generated.h"

/**
 * 
 */

class UAWeekInteractionWidget;
class UAWeekInventoryMainPanel;
class UAWeekCraftingMainPanel;
class UAWeekHeldItemVisual;

UCLASS(BlueprintType)
class AWEEK_API UAWeekUIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekInventoryMainPanel> InventoryMainPanelClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekCraftingMainPanel> CraftingMainPanelClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekHeldItemVisual> HeldItemVisualClass;
};