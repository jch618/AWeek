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
	UAWeekInventoryPanel();
	
	FORCEINLINE UAWeekInventoryComponent* GetInventory() const { return InventoryComponent; }

	UFUNCTION()
	void RefreshInventory();
	
	void LinkToInventory(TObjectPtr<UAWeekInventoryComponent> InInventoryComponent);
	void UnlinkFromInventory();

	FORCEINLINE bool IsLinkedToInventory() const { return bIsLinkedToInventory; }
	
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
	UAWeekInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAWeekInventoryItemSlot> InventorySlotClass;

	UPROPERTY(EditDefaultsOnly)
	int32 NumCols;

	virtual void NativeOnInitialized() override;
	
	void UpdateInfoText() const;

	virtual void RefreshInventoryPanel();
	virtual void InitializeGridPanel();
	virtual void OnInventoryLinked();
	virtual void RefreshAdditionalUI() {}

private:
	UFUNCTION()
	void OnEncumberedStatusChanged(bool bIsEncumbered) const;

	bool bIsLinkedToInventory;
};
