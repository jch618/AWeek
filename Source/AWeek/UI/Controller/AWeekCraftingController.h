#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AWeekCraftingController.generated.h"

class UAWeekCraftingComponent;
class UAWeekInventoryComponent;
class UAWeekGameUIManager;
struct FAWeekCachedCraftingRecipe;

UENUM(BlueprintType)
enum class ECraftingFailureReason : uint8
{
    Success,
    InvalidRecipe,
    InsufficientMaterials,
    InventoryFull,
    ComponentNotInitialized
};

USTRUCT(BlueprintType)
struct FCraftingResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ECraftingFailureReason Result = ECraftingFailureReason::ComponentNotInitialized;

    UPROPERTY(BlueprintReadOnly)
    FText Message;

    UPROPERTY(BlueprintReadOnly)
    int32 CraftedCount = 0;

    static FCraftingResult MakeSuccess(int32 Count, const FText& ItemName)
    {
        FCraftingResult Result;
        Result.Result = ECraftingFailureReason::Success;
        Result.CraftedCount = Count;
        Result.Message = FText::Format(
            FText::FromString(TEXT("Successfully crafted {0}x {1}")),
            FText::AsNumber(Count), ItemName);
        return Result;
    }

    static FCraftingResult MakeFailure(ECraftingFailureReason Reason, const FText& Message)
    {
        FCraftingResult Result;
        Result.Result = Reason;
        Result.Message = Message;
        return Result;
    }
};

UCLASS()
class AWEEK_API UAWeekCraftingController : public UObject
{
    GENERATED_BODY()

public:
    //================================================================
    // Initialization
    //================================================================
    void InitializeCraftingController(
        TObjectPtr<UAWeekGameUIManager> InUIManager,
        TObjectPtr<UAWeekCraftingComponent> InCraftingComponent,
        TObjectPtr<UAWeekInventoryComponent> InInventoryComponent);

    //================================================================
    // Crafting Actions
    //================================================================
    
    UFUNCTION(BlueprintCallable)
    FCraftingResult TryCraftRecipe(int32 RecipeIndex, int32 CraftCount = 1);

    UFUNCTION(BlueprintCallable)
    bool CanCraftRecipe(int32 RecipeIndex) const;

    //================================================================
    // Getters
    //================================================================
    FORCEINLINE UAWeekCraftingComponent* GetCraftingComponent() const { return CraftingComponent; }

private:
    UPROPERTY()
    TObjectPtr<UAWeekGameUIManager> UIManager;

    UPROPERTY()
    TObjectPtr<UAWeekCraftingComponent> CraftingComponent;

    UPROPERTY()
    TObjectPtr<UAWeekInventoryComponent> InventoryComponent;

    FCraftingResult ValidateCrafting(int32 RecipeIndex, int32 CraftCount) const;
};