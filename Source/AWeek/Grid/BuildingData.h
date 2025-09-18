#pragma once

#include "CoreMinimal.h"
#include "BuildingData.generated.h"

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	None UMETA(DisplayName = "None"),
	Wall UMETA(DisplayName = "Wall"),
	Floor UMETA(DisplayName = "Floor"),
	Roof UMETA(DisplayName = "Roof"),
	Fence UMETA(DisplayName = "Fence"),
	Interaction UMETA(DisplayName = "Interaction"),
	
};
USTRUCT(BlueprintType)
struct FBuildingSize
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size", meta=(ClampMin="0"))
	FVector Size = FVector(1.f, 1.f, 1.f);
	
};


USTRUCT(BlueprintType)
struct FBuildingStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat", meta=(ClampMin="0.0"))
	float CurrentHealth = 1.f;
	float MaxHealth = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	EBuildingType BuildingType = EBuildingType::None;
	
};