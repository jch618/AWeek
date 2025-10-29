#pragma once

#include "CoreMinimal.h"
#include "AWeek/Grid/PreviewObject.h"
#include "AWeekItemDataStructs.h"
#include "Engine/DataTable.h"
#include "AWeekBuildingData.generated.h"

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	Wall UMETA(DisplayName = "Wall"),
	Turret UMETA(DisplayName = "Turret"),
	Bench UMETA(DisplayName = "Bench"),
};


USTRUCT(BlueprintType)
struct FAWeekCost
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cost")
	FDataTableRowHandle ItemRow; // DT_Items의 Row
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cost", meta=(ClampMin="1"))
	int32 Amount = 1;
};

USTRUCT(BlueprintType)
struct FAWeekBuildingData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Building Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Building Data")
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, Category = "Building Data")
	EBuildingType Type;

	UPROPERTY(EditAnywhere, Category = "Building Data")
	TSoftClassPtr<APreviewObject> PreviewObjectClass;

	UPROPERTY(EditAnywhere, Category = "Building Data")
	TSoftClassPtr<AGridPlacedActor> GridPlacedActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Data")
	TArray<FAWeekCost> Costs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Building Data")
	FString BuildingText;
};
