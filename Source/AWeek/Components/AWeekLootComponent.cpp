#include "AWeek/Components/AWeekLootComponent.h"

#include "AWeekInventoryComponent.h"
#include "AWeek/Data/AWeekItemDataStructs.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/World/AWeekLootChest.h"
#include "AWeek/Data/AWeekLootItemData.h"

// AWeekLootDropComponent.cpp
UAWeekLootComponent::UAWeekLootComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAWeekLootComponent::DropLoot(const FVector& Location)
{
    UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
    
    TArray<FAWeekItemEntry> GeneratedLoot = GenerateRandomLoot();
    
    if (GeneratedLoot.Num() == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("No loot generated"));
        return;
    }
    SpawnLootChest(Location, GeneratedLoot);
}

TArray<FAWeekItemEntry> UAWeekLootComponent::GenerateRandomLoot()
{
    if (LootTableRowHandle.IsNull())
    {
        UE_LOG(LogTemp, Log, TEXT("%s: row handle is null"), *FString(__FUNCTION__));
        return TArray<FAWeekItemEntry>();
    }
    
    const FAWeekLootItemData* LootDropItemData = LootTableRowHandle.GetRow<FAWeekLootItemData>(LootTableRowHandle.RowName.ToString());
    if (!LootDropItemData)
    {
        UE_LOG(LogTemp, Log, TEXT("%s: LootDropItemData is null"), *FString(__FUNCTION__));
        return TArray<FAWeekItemEntry>();
    }
    
    TArray<FAWeekItemEntry> GeneratedItems;
    
    int32 NumItemsToDrop = FMath::RandRange(LootDropItemData->MinItemsToDrop, LootDropItemData->MaxItemsToDrop);

    for (const FAWeekLootItemEntry& Entry : LootDropItemData->PossibleLootItemEntries)
    {
        if (GeneratedItems.Num() >= NumItemsToDrop)
            break;
        
        float RandomChance = FMath::FRandRange(0.0f, 100.0f);
        if (RandomChance > Entry.DropChance)
            continue;
        
        const FAWeekItemData* ItemData = Entry.ItemHandle.GetRow<FAWeekItemData>(Entry.ItemHandle.RowName.ToString());
        if (!ItemData)
        {
            UE_LOG(LogTemp, Log, TEXT("%s: ItemData is null"), *FString(__FUNCTION__));
            continue;
        }
        
        int32 Quantity = FMath::RandRange(Entry.MinQuantity, Entry.MaxQuantity);

        GeneratedItems.Add(FAWeekItemEntry(*ItemData, Quantity));
    }

    return GeneratedItems;
}

void UAWeekLootComponent::SpawnLootChest(const FVector& Location, const TArray<FAWeekItemEntry>& Items)
{
    if (!LootChestClass)
    {
        UE_LOG(LogTemp, Error, TEXT("LootChestClass is not set!"));
        return;
    }

    // FActorSpawnParameters SpawnParams;
    // SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    UE_LOG(LogTemp, Warning, TEXT("%s: Spawn Actor"), *FString(__FUNCTION__));
    
    AAWeekLootChest* LootChest = GetWorld()->SpawnActor<AAWeekLootChest>(
        LootChestClass,
        Location,
        FRotator::ZeroRotator
    );

    if (LootChest)
    {
        UAWeekInventoryComponent* ChestInventory = LootChest->GetInventoryComponent();
        if (ChestInventory)
        {
            for (const FAWeekItemEntry& ItemEntry : Items)
            {
                UAWeekItemBase* NewItem = UAWeekItemBase::CreateFromData(
                    ItemEntry.ItemData,
                    ItemEntry.Quantity,
                    ChestInventory
                );

                if (NewItem)
                {
                    ChestInventory->HandleAddItem(NewItem);
                }
            }
        }

        UE_LOG(LogTemp, Log, TEXT("Spawned LootChest with %d items at %s"), 
               Items.Num(), *Location.ToString());
    }
}
