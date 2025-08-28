//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "AWeekAWeekInventoryComponent.generated.h"
//
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class AWEEK_API UAWeekAWeekInventoryComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//	// Sets default values for this component's properties
//	UAWeekAWeekInventoryComponent();
//
//protected:
//	// Called when the game starts
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//		
//};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated)

class UAWeekItemBase;
UENUM(BluePrintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added")
};

USTRUCT()
struct FItemSlot
{
	GENERATED_BODY()

	FItemSlot(int32 NumRow, int32 NumCol, UAWeekItemBase* ItemReference, UAWeekInventoryComponent* Inventory) :
		Row(NumRow), Col(NumCol), Item(ItemReference), OwningInventory(Inventory)
	{
		bIsEmpty = (Item == nullptr);
	}
	FItemSlot() :
		Row(0), Col(0), Item(nullptr), bIsEmpty(true), OwningInventory(nullptr)
	{
	}
	int32 Row;
	int32 Col;
	UAWeekItemBase* Item;
	bool bIsEmpty;
	UAWeekInventoryComponent* OwningInventory;
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
		ActualAmountAdded(0),
		OperationResult(EItemAddResult::IAR_NoItemAdded),
		ResultMessage(FText::GetEmpty())
	{
	}

	// Actual amount of item that was added to the inventory
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	// Enum representing the end state of an add-item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	// Informational message that can be passed with the result
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	}

	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	}

};
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AWEEK_API UAWeekInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	FOnInventoryUpdated OnInventoryUpdated;


	//================================================================
	//	FUNCTIONS
	//================================================================
	UAWeekInventoryComponent();

	FORCEINLINE bool IsValidItemSlotIndex(const int32 ItemSlotIndex) { return InventoryContents.IsValidIndex(ItemSlotIndex); }
	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UAWeekItemBase* InputItem);

	UFUNCTION(Category = "Inventory")
	UAWeekItemBase* FindMatchingItem(UAWeekItemBase* ItemIn) const;
	UFUNCTION(Category = "Inventory")
	UAWeekItemBase* FindNextItemByID(UAWeekItemBase* ItemIn) const;
	FItemSlot* FindNextPartialStack(UAWeekItemBase* ItemIn);

	UFUNCTION(Category = "Inventory")
	void ClearItemSlot(FItemSlot& ItemToRemove);
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(FItemSlot& ItemSlot, int32 DesiredAmountToRemove);
	int32 RemoveAmountOfItem(const FItemSlot& ItemSlot, int32 DesiredAmountToRemove);
	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(FItemSlot& ItemSlot, const int32 AmountToSplit);

	void SwapItemSlotWith(const int32 ItemSlotIndex, const int32 OtherItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OtherInventory);

	// getters
	// -------------------------------------
	UFUNCTION(Category = "Inventory")
	int32 GetEmptySlotsNum();

	UFUNCTION(Category = "Inventory")

	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; }
	UFUNCTION(Category = "Inventory")

	FORCEINLINE float GetWeightCapacity() { return InventoryWeightCapacity; }
	UFUNCTION(Category = "Inventory")

	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; }
	UFUNCTION(Category = "Inventory")

	FORCEINLINE TArray<FItemSlot>& GetInventoryContents() { return InventoryContents; }
	FORCEINLINE const TArray<FItemSlot>& GetInventoryContents() const { return InventoryContents; }

	FORCEINLINE const FItemSlot& GetItemSlotAt(int32 Index) const { return InventoryContents[Index]; }

	// setters
	// -------------------------------------
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; }
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; }

protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float InventoryTotalWeight;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 InventorySlotsCapacity;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float InventoryWeightCapacity;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	//TArray<TObjectPtr<UAWeekItemBase>> InventoryContents;
	TArray<FItemSlot> InventoryContents;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 NumRows;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 NumCols;
	//================================================================
	//	FUNCTIONS
	//================================================================
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UAWeekItemBase* InputItem);
	int32 HandleStackableItems(UAWeekItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UAWeekItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UAWeekItemBase* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UAWeekItemBase* Item, const int32 AmountToAdd);
private:
	void SetItemQuantity(FItemSlot& ItemSlot, const int32 Quantity);
	int32 GetFirstEmptySlotIndex();
	int32 FindItemIndex(const UAWeekItemBase* ItemIn) const;
};
