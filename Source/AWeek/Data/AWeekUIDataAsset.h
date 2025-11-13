// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AWeekUIDataAsset.generated.h"

/**
 * 
 */

class UAWeekInteractionWidget;
class UAWeekInventoryHubWidget;
class UAWeekInventoryMainPanel;
class UAWeekCraftingMainPanel;
class UAWeekHeldItemVisual;
class UAWeekInventoryController;
class UAWeekCraftingController;
class UMainUIWidget;
class UBuildingSelectWidget;

UCLASS(BlueprintType)
class AWEEK_API UAWeekUIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UAWeekInventoryHubWidget> InventoryHubWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UAWeekInventoryMainPanel> InventoryMainPanelClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UAWeekCraftingMainPanel> CraftingMainPanelClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UAWeekInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UAWeekHeldItemVisual> HeldItemVisualClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UMainUIWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Class")
	TSubclassOf<UBuildingSelectWidget> BuildingSelectWidgetClass;
};