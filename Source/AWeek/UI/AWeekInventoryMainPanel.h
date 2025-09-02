#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekInventoryMainPanel.generated.h"

class AAWeekPlayerCharacter;
class UAWeekInventoryPanel;
class UAWeekInventoryComponent;

UCLASS()
class AWEEK_API UAWeekInventoryMainPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UAWeekInventoryPanel> InventoryPanelClass;

	UPROPERTY()
	TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekInventoryPanel> PlayerInventoryPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekInventoryPanel> ChestInventoryPanel;

	void InitializeInventoryMainPanel();
	void ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> InventoryComponent);
	void DeActivateChestInventory();
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
