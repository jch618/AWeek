// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AWeek/Data/AWeekItemDataStructs.h"
#include "AWeekItemBase.generated.h"

class UInventoryComponent;
class AAWeekPlayerCharacter;

UCLASS()
class AWEEK_API UAWeekItemBase : public UObject
{
	GENERATED_BODY()

public:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================


	//================================================================
	//	FUNCTIONS
	//================================================================
	UAWeekItemBase();

	// getters and setters
	FORCEINLINE FName GetID() const { return ItemData.ID; }
	FORCEINLINE int GetQuantity() const { return Quantity; }
	FORCEINLINE EAWeekItemType GetItemType() const { return ItemData.ItemType; }
	// FORCEINLINE EAWeekItemQuality GetItemQuality() const { return ItemData.ItemQuality; }
	FORCEINLINE const FAWeekItemTextData& GetTextData() const { return ItemData.TextData; }
	FORCEINLINE const FAWeekItemAssetData& GetAssetData() const { return ItemData.AssetData; }
	FORCEINLINE const FAWeekItemNumericData& GetNumericData() const { return ItemData.NumericData; }
	// FORCEINLINE const FAWeekItemStatistics& GetStatistics() const { return ItemData.ItemStatistics; }
	FORCEINLINE const FAWeekItemData& GetItemData() const { return ItemData; }

	FORCEINLINE bool IsCopy() const { return bIsCopy; }
	FORCEINLINE bool IsPickup() const { return bIsPickup; }

	virtual void InitializeFromItemData(const FAWeekItemData& InItemData, int32 InQuantity);
	
	UFUNCTION(Category = "Item")
	UAWeekItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * ItemData.NumericData.Weight; }

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return ItemData.NumericData.Weight; }

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == ItemData.NumericData.MaxStackSize; }

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 InQuantity);

	virtual bool UsePrimary(TObjectPtr<AAWeekPlayerCharacter> Character);
	virtual bool UseSecondary(TObjectPtr<AAWeekPlayerCharacter> Character);

	void ResetItemFlags();

	static UAWeekItemBase* CreateFromData(const FAWeekItemData& InItemData, int32 InQuantity, UObject* Outer);
	static UAWeekItemBase* CreateFromRowHandle(const FDataTableRowHandle& RowHandle, int32 InQuantity, UObject* Outer);

protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY(VisibleAnywhere, Category = "Item")
	FAWeekItemData ItemData;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	bool bIsCopy;
	bool bIsPickup;
	//================================================================
	//	FUNCTIONS
	//================================================================
	bool operator==(const FName& OtherID) const
	{
		return ItemData.ID == OtherID;
	}
	
private:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================

	
	//================================================================
	//	FUNCTIONS
	//================================================================
};
