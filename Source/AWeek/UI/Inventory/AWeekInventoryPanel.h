#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekInventoryPanel.generated.h"

class UUniformGridPanel;
class UWrapBox;
class UTextBlock;

class UAWeekInventoryItemSlot;
class UAWeekInventoryComponent;
class AAWeekPlayerCharacter;

UCLASS()
class AWEEK_API UAWeekInventoryPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	bool bIsLinkedToInventory;

	UFUNCTION()
	void RefreshInventory();
	void LinkToInventory(TObjectPtr<UAWeekInventoryComponent> InputInventory, TObjectPtr<AAWeekPlayerCharacter> InputCharacter = nullptr);
	void UnlinkFromInventory();

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InventoryTitle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;

	//UPROPERTY(meta = (BindWidget))
	//UWrapBox* InventoryWrapBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeightInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CapacityInfo;

	UPROPERTY()
	AAWeekPlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UAWeekInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAWeekInventoryItemSlot> InventorySlotClass;


	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
