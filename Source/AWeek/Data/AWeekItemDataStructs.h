#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AWeekItemDataStructs.generated.h"

// UENUM()
// enum class EAWeekItemQuality : uint8
// {
// 	Poor UMETA(DisplayName = "Poor"),
// 	Common UMETA(DisplayName = "Common"),
// 	Rare UMETA(DisplayName = "Rare"),
// 	Epic UMETA(DisplayName = "Epic"),
// 	Grandmaster UMETA(DisplayName = "Grandmaster")
// };

UENUM()
enum class EAWeekItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Consumable UMETA(DisplayName = "Consumable"),
	Ammo UMETA(DisplayName = "Ammo"),
	Generic UMETA(DisplayName = "Generic")
};

USTRUCT()
struct FAWeekItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT()
struct FAWeekItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;
};

USTRUCT()
struct FAWeekItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT()
struct FAWeekConsumableData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 HealthDelta;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 StaminaDelta;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 HungerDelta;
};

USTRUCT()
struct FAWeekItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EAWeekItemType ItemType;

	// UPROPERTY(EditAnywhere, Category = "Item Data")
	// EAWeekItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FAWeekItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FAWeekItemNumericData NumericData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FAWeekItemAssetData AssetData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FAWeekConsumableData ConsumableData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName WeaponID;
};

USTRUCT()
struct FAWeekItemEntry
{
	GENERATED_BODY()

	UPROPERTY()
	FAWeekItemData ItemData;

	UPROPERTY()
	int32 Quantity;

	FAWeekItemEntry() = default;
	FAWeekItemEntry(const FAWeekItemData& InItemData, int32 InQuantity):
		ItemData(InItemData), Quantity(InQuantity) { }
};