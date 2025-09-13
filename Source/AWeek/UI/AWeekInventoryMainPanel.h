#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekInventoryMainPanel.generated.h"

class AAWeekPlayerCharacter;
class UAWeekInventoryPanel;
class UAWeekInventoryComponent;
class UAWeekDragItemVisual;

struct FAWeekItemSlot;

UCLASS()
class AWEEK_API UAWeekInventoryMainPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	//================================================================
	//	FUNCTIONS
	//================================================================
	FORCEINLINE bool IsChestOpen() const { return bIsChestOpen; }
	FORCEINLINE UAWeekInventoryComponent* GetPlayerInventoryComponent();
	FORCEINLINE UAWeekInventoryComponent* GetChestInventoryComponent();

	UFUNCTION()
	void InitializeInventoryMainPanel();
	void ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> InventoryComponent);
	void DeActivateChestInventory();
protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	bool bIsChestOpen;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UAWeekInventoryPanel> InventoryPanelClass;

	UPROPERTY()
	TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekInventoryPanel> PlayerInventoryPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekInventoryPanel> ChestInventoryPanel;

	//================================================================
	//	FUNCTIONS
	//================================================================
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
