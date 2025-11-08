#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "AWeek/UI/Inventory/AWeekItemSlot.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "AWeekInventoryItemSlot.generated.h"

class UTextBlock;
class UBorder;
class UImage;

class UAWeekHeldItemVisual;
class UAWeekItemBase;
class UAWeekInventoryToolTip;
struct FAWeekInventorySlotData;
class UAWeekInventoryComponent;

DECLARE_DELEGATE_OneParam(FOnShiftLeftClick, const FAWeekInventorySlotData& ClickedItemSlot);
DECLARE_DELEGATE_TwoParams(FOnLeftClick, int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);
DECLARE_DELEGATE_TwoParams(FOnRightClick, int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory);

UCLASS()
class AWEEK_API UAWeekInventoryItemSlot : public UAWeekItemSlot
{
	GENERATED_BODY()

public:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	FOnShiftLeftClick OnShiftLeftClick;
	FOnLeftClick OnLeftClick;
	FOnRightClick OnRightClick;

	FORCEINLINE void SetItemSlotIndex(int32 Index) { ItemSlotIndex = Index; }
	FORCEINLINE int32 GetItemSlotIndex() const { return ItemSlotIndex; }

	FORCEINLINE void SetInventory(TObjectPtr<UAWeekInventoryComponent> Inventory) { OwningInventory = Inventory; }
	const UAWeekItemBase* GetItem() const;

	//================================================================
	//	FUNCTIONS
	//================================================================
	void InitializeInventoryItemSlot(TObjectPtr<UAWeekItemBase> Item);

protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	// UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	// TSubclassOf<UAWeekInventoryToolTip> ToolTipClass;

	TWeakObjectPtr<UAWeekInventoryComponent> OwningInventory;
	int32 ItemSlotIndex;
	
	//================================================================
	//	FUNCTIONS
	//================================================================
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
