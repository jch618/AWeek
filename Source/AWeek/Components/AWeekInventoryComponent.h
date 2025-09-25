#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated)

class UAWeekItemBase;
UENUM(BluePrintType)
enum class EAWeekItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added")
};

USTRUCT()
struct FAWeekInventorySlotData
{
	GENERATED_BODY()

	FAWeekInventorySlotData(int32 NewItemSlotIndex, UAWeekInventoryComponent* NewOwningInventory, UAWeekItemBase* NewItem = nullptr) :
		bIsEmpty(NewItem == nullptr), ItemSlotIndex(NewItemSlotIndex), OwningInventory(NewOwningInventory), Item(NewItem)
	{ }
	FAWeekInventorySlotData() :
		bIsEmpty(true), ItemSlotIndex(-1), OwningInventory(nullptr), Item(nullptr)
	{ }

	bool bIsEmpty;
	
	int32 ItemSlotIndex;
	
	UPROPERTY()
	UAWeekInventoryComponent* OwningInventory;
	
	UPROPERTY()
	UAWeekItemBase* Item;
};

USTRUCT(BlueprintType)
struct FAWeekItemAddResult
{
	GENERATED_BODY()

	FAWeekItemAddResult() :
		ActualAmountAdded(0),
		OperationResult(EAWeekItemAddResult::IAR_NoItemAdded),
		ResultMessage(FText::GetEmpty())
	{
	}

	// Actual amount of item that was added to the inventory
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	// Enum representing the end state of an add-item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EAWeekItemAddResult OperationResult;

	// Informational message that can be passed with the result
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FAWeekItemAddResult AddedNone(const FText& ErrorText)
	{
		FAWeekItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EAWeekItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	}

	static FAWeekItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FAWeekItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;
		AddedPartialResult.OperationResult = EAWeekItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	}

	static FAWeekItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FAWeekItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EAWeekItemAddResult::IAR_AllItemAdded;
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

	FORCEINLINE bool IsValidItemSlotIndex(const int32 ItemSlotIndex) const { return InventoryContents.IsValidIndex(ItemSlotIndex); }

	FORCEINLINE bool CanAddItemWeight(const int32 NewWeight) const { return InventoryTotalWeight + NewWeight <= InventoryWeightCapacity; }

	UFUNCTION(Category = "Inventory")
	FAWeekItemAddResult HandleAddItem(UAWeekItemBase* InputItem);

	UFUNCTION(Category = "Inventory")
	UAWeekItemBase* FindMatchingItem(UAWeekItemBase* ItemIn) const;
	UFUNCTION(Category = "Inventory")
	int32 FindNextItemByID(FName ItemID) const;
	FAWeekInventorySlotData* FindNextPartialStack(UAWeekItemBase* ItemIn);

	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(FAWeekInventorySlotData& ItemSlot, int32 DesiredAmountToRemove);
	int32 RemoveAmountOfItem(int32 TargetItemSlotIndex, int32 DesiredAmountToRemove);
	bool TryRemoveAmountOfItem(FName ItemID, int32 DesiredAmountToRemove);
	UAWeekItemBase* ReleaseItemAt(int32 TargetItemSlotIndex);
	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(FAWeekInventorySlotData& ItemSlot, const int32 AmountToSplit);

	void SwapItemSlotWith(const int32 ItemSlotIndex, const int32 OtherItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OtherInventory);
	int32 GetFirstEmptySlotIndex();

	// getters
	// -------------------------------------
	UFUNCTION(Category = "Inventory")
	int32 GetEmptySlotsNum() const;

	UFUNCTION(Category = "Inventory")

	FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; }
	UFUNCTION(Category = "Inventory")

	FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; }
	UFUNCTION(Category = "Inventory")

	FORCEINLINE int32 GetSlotsCapacity() const { return InventorySlotsCapacity; }
	UFUNCTION(Category = "Inventory")

	FORCEINLINE TArray<FAWeekInventorySlotData>& GetInventoryContents() { return InventoryContents; }
	FORCEINLINE const TArray<FAWeekInventorySlotData>& GetInventoryContents() const { return InventoryContents; }

	FORCEINLINE const FAWeekInventorySlotData& GetItemSlotAt(int32 Index) const { return InventoryContents[Index]; }
	FORCEINLINE bool IsLinkedToInventoryPanel() const { return bIsLinkedToInventoryPanel; }

	// setters
	// -------------------------------------
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; }
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; }
	FORCEINLINE void SetIsLinkedToInventoryPanel(bool bNewIsLinkedToInventoryPanel)
	{
		bIsLinkedToInventoryPanel = bNewIsLinkedToInventoryPanel;
	}
	FORCEINLINE void ClearInventoryContents() { InventoryContents.Empty(); }
	int32 AddItemQuantityAt(int32 ItemSlotIndex, int32 DesiredAddAmount);
	void PlaceItemAt(TObjectPtr<UAWeekItemBase> InputItem, int32 TargetIndex);

	void TransferItem(const FAWeekInventorySlotData& FromItemSlot, TObjectPtr<UAWeekInventoryComponent> TargetInventory);

	// crafting
	TMap<FName, int32> GetInventoryItemCounts() const;

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
	TArray<FAWeekInventorySlotData> InventoryContents;

	bool bIsLinkedToInventoryPanel;
	//================================================================
	//	FUNCTIONS
	//================================================================
	virtual void BeginPlay() override;

	void ClearItemSlot(FAWeekInventorySlotData& ItemSlotToRemove);

	FAWeekItemAddResult HandleNonStackableItems(UAWeekItemBase* InputItem);
	int32 HandleStackableItems(UAWeekItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateWeightAddAmount(UAWeekItemBase* ItemIn, int32 RequestedAddAmount);
	int32 CalculateNumberForFullStack(UAWeekItemBase* StackableItem, int32 InitialRequestedAddAmount);

	void AddNewItem(UAWeekItemBase* Item, const int32 AmountToAdd, int32 TargetIndex = -1);
private:
	void SetItemQuantity(FAWeekInventorySlotData& ItemSlot, const int32 Quantity);
	int32 FindItemIndex(const UAWeekItemBase* ItemIn) const;
};
