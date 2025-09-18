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
	//UPROPERTY()
	//UInventoryComponent* OwningInventory;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EAWeekItemType ItemType;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	EAWeekItemQuality ItemQuality;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FAWeekItemStatistics ItemStatistics;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FAWeekItemTextData TextData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FAWeekItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FAWeekItemAssetData AssetData;

	bool bIsCopy;
	bool bIsPickup;

	//================================================================
	//	FUNCTIONS
	//================================================================
	UAWeekItemBase();

	UFUNCTION(Category = "Item")
	UAWeekItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; }

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; }

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; }

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AAWeekPlayerCharacter* Character);

	void ResetItemFlags();

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	}

private:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================

	//================================================================
	//	FUNCTIONS
	//================================================================
};
