// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekInventoryPanel.h"
#include "AWeek/Components/AWeekPlayerInventoryComponent.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeekInventoryHubWidget.generated.h"

class UAWeekPlayerInventoryPanel;
enum class EAWeekInventoryHubPanel : uint8;
class UCommonButtonBase;
class UAWeekCraftingController;
class UAWeekInventoryComponent;
class UAWeekInventoryPanel;
class UCommonActivatableWidgetSwitcher;
class UAWeekCraftingComponent;
class UAWeekCraftingDetailPanel;
class UAWeekCraftingListPanel;
class AAWeekPlayerCharacter;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FAWeekPanelContext
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAWeekInventoryComponent> ChestInventoryComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAWeekCraftingComponent> CraftingComponent = nullptr;

	UPROPERTY()
	int32 CraftingTableLevel = 0;

	FAWeekPanelContext() = default;
	
	static FAWeekPanelContext ForChest(TObjectPtr<UAWeekInventoryComponent> ChestInventoryComponent)
	{
		FAWeekPanelContext Context;
		Context.ChestInventoryComponent = ChestInventoryComponent;
		return Context;
	}
	
	static FAWeekPanelContext ForCrafting(TObjectPtr<UAWeekCraftingComponent> CraftingComponent)
	{
		FAWeekPanelContext Context;
		Context.CraftingComponent = CraftingComponent;
		return Context;
	}
};

UCLASS()
class AWEEK_API UAWeekInventoryHubWidget : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	//================================================================
	//	FUNCTIONS
	//================================================================
	void InitializeInventoryHub(TObjectPtr<UAWeekCraftingController> InCraftingController, TObjectPtr<UAWeekPlayerInventoryComponent>
	                            InPlayerInventoryComponent);

	/* Control panel */
	UFUNCTION(BlueprintCallable, Category = "Inventory Hub")
	void SwitchToPanel(EAWeekInventoryHubPanel TargetPanel, const FAWeekPanelContext& PanelContext);

	UFUNCTION(BlueprintCallable, Category = "Inventory Hub")
	void CloseCurrentPanel();

	FORCEINLINE EAWeekInventoryHubPanel GetCurrentPanel() const { return CurrentPanel; }
	FORCEINLINE bool IsPanelOpen(EAWeekInventoryHubPanel Panel) const { return CurrentPanel == Panel; }
	FORCEINLINE bool IsChestOpen() const
	{
		return ChestInventoryPanel->IsLinkedToInventory();
	}
	void CloseChestInventory();

	void ShowCraftingDetailPanel() const;
	void HideCraftingDetailPanel() const;
	
protected:
	//================================================================
	//	FUNCTIONS
	//================================================================
	// virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
private:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================

	/* Widgets */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekPlayerInventoryPanel> PlayerInventoryPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetSwitcher> PanelSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekInventoryPanel> ChestInventoryPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekCraftingListPanel> CraftingListPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekCraftingDetailPanel> CraftingDetailPanel;

	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<UUserWidget> EmptyWidget;

	/* Panel */
	UPROPERTY(VisibleAnywhere, Category = "Inventory Hub")
	EAWeekInventoryHubPanel CurrentPanel;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Hub")
	EAWeekInventoryHubPanel PreviousPanel;

	/* Buttons */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> ChestPanelButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> CraftingPanelButton;

	// References
	UPROPERTY()
	TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UAWeekCraftingController> CraftingController;
	//================================================================
	//	FUNCTIONS
	//================================================================
	/* Initialize */
	void InitializeInventoryPanel() const;
	void InitializeCraftingPanel();
	
	/* Crafting Panel handlers */
	UFUNCTION()
	void OnRecipeSelected(int32 RecipeIndex, bool bIsCraftable);

	UFUNCTION()
	void OnCraftButtonLeftClicked(int32 RecipeIndex) const;

	UFUNCTION()
	void RefreshInventoryHub();

	/* Panel control */
	void ActivatePanel(EAWeekInventoryHubPanel Panel, const FAWeekPanelContext& Context);
	void DeactivatePanel(EAWeekInventoryHubPanel Panel);

	void UpdatePanelButtons();
	
};
