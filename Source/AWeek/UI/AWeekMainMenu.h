// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekMainMenu.generated.h"

class AAWeekPlayerCharacter;
class UAWeekInventoryPanel;
class UAWeekInventoryComponent;

UCLASS()
class AWEEK_API UAWeekMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekInventoryPanel> PlayerInventoryPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekInventoryPanel> ChestInventoryPanel;
	void ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> InventoryComponent);
	void DeActivateChestInventory();
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
