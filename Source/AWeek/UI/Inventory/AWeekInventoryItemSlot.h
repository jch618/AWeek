#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekInventoryItemSlot.generated.h"


class UTextBlock;
class UBorder;
class UImage;

class UAWeekDragItemVisual;
class UAWeekItemBase;
class UAWeekInventoryToolTip;
struct FAWeekItemSlot;
class UAWeekInventoryComponent;

UCLASS()
class AWEEK_API UAWeekInventoryItemSlot : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemSlotIndex(int32 Index) { ItemSlotIndex = Index; }
	FORCEINLINE int32 GetItemSlotIndex() const { return ItemSlotIndex; }

	FORCEINLINE void SetInventory(TObjectPtr<UAWeekInventoryComponent> Inventory) { OwningInventory = Inventory; }
	const TObjectPtr<UAWeekItemBase> GetItemReference() const;

	void InitializeItemSlot();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UAWeekDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UAWeekInventoryToolTip> ToolTipClass;

	TObjectPtr<UAWeekInventoryComponent> OwningInventory;
	int32 ItemSlotIndex;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
