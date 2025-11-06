// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AWeekGameUIManager.generated.h"

class UAWeekInventoryHubWidget;
class UAWeekInventoryController;
class UAWeekCraftingComponent;
class UAWeekCraftingMainPanel;
class UAWeekUIDataAsset;
class AAWeekPlayerController;
class UAWeekInventoryMainPanel;
class UMainUIWidget;
class UPreviewObjectWidget;
class UAWeekInteractionWidget;
struct FAWeekInteractableData;
class UAWeekInventoryComponent;
class UAWeekHeldItemVisual;
class UAWeekHeldItem;
class UAWeekItemBase;
struct FAWeekInventorySlotData;
class AAWeekPlayerCharacter;
class UAWeekCraftingController;

/**
 * 
 */
UENUM()
enum class EAWeekInventoryHubPanel : uint8
{
	None UMETA(DisplayName = "None"),
	Weapon UMETA(DisplayName = "Weapon Panel"),
	Chest UMETA(DisplayName = "Chest Inventory Panel"),
	Crafting UMETA(DisplayName = "Crafting Inventory Panel"),
	PlayerState UMETA(DisplayName = "Player State Panel"),
};

UCLASS()
class AWEEK_API UAWeekGameUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	// =====================================================
	// INVENTORY SYSTEM
	// =====================================================

	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================

	//================================================================
	//	FUNCTIONS
	//================================================================

public:
	UAWeekGameUIManager();
	
	void InitializeUIManager(TObjectPtr<AAWeekPlayerCharacter> InPlayerCharacter);

	void ToggleInventoryHub(EAWeekInventoryHubPanel DisplayPanel);
	// void ToggleInventoryMainPanel();
	void ToggleMainWidget();
	// void ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);
	//
	// void ToggleCraftingMainPanel(TObjectPtr<UAWeekCraftingComponent> CraftingComponent, const TObjectPtr<UAWeekInventoryComponent> InventoryComponent);

	void PreviewObjectRotateL();
	void PreviewObjectRotateR();

	void ShowInteractionWidget();
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FAWeekInteractableData* InteractableData);

	void OpenChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory) const;
	void CloseChestInventory();

	// void HideCraftingMainPanel();
	
	FORCEINLINE UAWeekInventoryController* GetInventoryController() const { return InventoryController; }
	FORCEINLINE AAWeekPlayerCharacter* GetPlayerCharacter() const { return PlayerCharacter; }
	FORCEINLINE AAWeekPlayerController* GetPlayerContoller() const { return PlayerController; }
protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	
	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekInventoryMainPanel> InventoryMainPanelClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekCraftingMainPanel> CraftingMainPanelClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekInventoryHubWidget> InventoryHubWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UAWeekHeldItemVisual> HeldItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI Classes")
	TSubclassOf<UMainUIWidget> MainUIWidgetClass;
	UPROPERTY()
	TObjectPtr<UMainUIWidget> MainUIWidget;

	UPROPERTY()
	TObjectPtr<UAWeekInventoryHubWidget> InventoryHubWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPreviewObjectWidget> PreviewObjectWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TObjectPtr<UPreviewObjectWidget> PreviewObjectWidget;
	
	UPROPERTY()
	TObjectPtr<UAWeekInventoryMainPanel> InventoryMainPanelWidget;

	UPROPERTY()
	TObjectPtr<UAWeekCraftingMainPanel> CraftingMainPanelWidget;;
	
	UPROPERTY()
	TObjectPtr<UAWeekInteractionWidget> InteractionWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> CrosshairWidget;

	UPROPERTY()
	TObjectPtr<ULocalPlayer> LocalPlayer;

	UPROPERTY()
	TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<AAWeekPlayerController> PlayerController;

	void OpenInventoryHub(EAWeekInventoryHubPanel DisplayPanel);
	void CloseInventoryHub();

	void ShowMainWidget();
	void HideMainWidget();
	void ShowCraftingMainPanel();

	void ShowCraftingMainPanel(TObjectPtr<UAWeekCraftingComponent> CraftingComponent, const TObjectPtr<UAWeekInventoryComponent> InventoryComponent);

private:
	FORCEINLINE bool IsInventoryHubOpen() const;
	UPROPERTY(EditAnywhere, Category = "UI Settings")
	FSoftObjectPath UIDataAssetPath;
    
	UPROPERTY()
	UAWeekUIDataAsset* UIDataAsset;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
	TObjectPtr<UAWeekInventoryController> InventoryController;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
	TObjectPtr<UAWeekCraftingController> CraftingController;
};