// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "AWeekPlayerController.generated.h"

class UAWeekInventoryMainPanel;
class UAWeekInteractionWidget;
struct FAWeekInteractableData;
class UAWeekInventoryComponent;
/**
 * 
 */
UCLASS()
class AWEEK_API AAWeekPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()
public:
	AAWeekPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// =====================================================
	// INVENTORY SYSTEM
	// =====================================================

	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAWeekInventoryMainPanel> InventoryMainPanelClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UAWeekInteractionWidget> InteractionWidgetClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	//TSubclassOf<UUserWidget> CrosshairWidgetClass;

	//================================================================
	//	FUNCTIONS
	//================================================================

	void ShowMainPanel();
	void HideMainPanel();
	void ToggleMainPanel();
	void ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);

	void ShowCrosshair();
	void HideCrosshair();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FAWeekInteractableData* InteracableData);

	void ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory);
	void DeactivateChestInventory();

protected:
	TSubclassOf<UUserWidget> mMainWidget;

protected:
	virtual void BeginPlay() override;

protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY()
	TObjectPtr<UAWeekInventoryMainPanel> InventoryMainPanelWidget;

	UPROPERTY()
	TObjectPtr<UAWeekInteractionWidget> InteractionWidget;

	UPROPERTY()
	TObjectPtr<UUserWidget> CrosshairWidget;

private:
	bool bIsChestOpen;
};
