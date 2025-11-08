#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekLootComponent.generated.h"

class AAWeekLootChest;
struct FAWeekItemEntry;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AWEEK_API UAWeekLootComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAWeekLootComponent();
	
	UFUNCTION(BlueprintCallable)
	void DropLoot(const FVector& Location);

protected:
	UPROPERTY(EditAnywhere, Category = "Loot")
	FDataTableRowHandle LootTableRowHandle;
	
	UPROPERTY(EditAnywhere, Category = "Loot")
	TSubclassOf<AAWeekLootChest> LootChestClass;
	
private:
	TArray<FAWeekItemEntry> GenerateRandomLoot();
	void SpawnLootChest(const FVector& Location, const TArray<FAWeekItemEntry>& Items);
};