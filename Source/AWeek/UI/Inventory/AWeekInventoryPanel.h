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

struct FAWeekInventorySlotData;

DECLARE_DELEGATE_OneParam(FOnShiftClick, const FAWeekInventorySlotData& ClickedItemSlot);

UCLASS()
class AWEEK_API UAWeekInventoryPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	FOnShiftClick OnShiftClick;
	bool bIsLinkedToInventory;

	UAWeekInventoryPanel();
	FORCEINLINE UAWeekInventoryComponent* GetInventory() { return Inventory; }
	UFUNCTION()
	void RefreshInventory();
	void LinkToInventory(TObjectPtr<UAWeekInventoryComponent> InputInventory, TObjectPtr<AAWeekPlayerCharacter> InputCharacter = nullptr);
	void UnlinkFromInventory();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InventoryTitle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeightInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CapacityInfo;

	UPROPERTY()
	AAWeekPlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UAWeekInventoryComponent* Inventory;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAWeekInventoryItemSlot> InventorySlotClass;

	UPROPERTY(EditDefaultsOnly)
	int32 NumCols;
	
	void HandleShiftClickOnSlot(const FAWeekInventorySlotData& ClickedItemSlot);

	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
