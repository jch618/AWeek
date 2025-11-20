// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/World/AWeekChest.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Data/AWeekLootItemData.h"
#include "AWeek/Items/AWeekItemBase.h"

#include "Components/SphereComponent.h"

AAWeekChest::AAWeekChest() : ChestExitRadius(250.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	SetRootComponent(ChestMesh);

	InventoryComponent = CreateDefaultSubobject<UAWeekInventoryComponent>("ChestInventory");

	ChestExitSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	ChestExitSphere->SetupAttachment(RootComponent);
	ChestExitSphere->InitSphereRadius(ChestExitRadius);
}

void AAWeekChest::InitializeChest()
{
	if (InventoryComponent)
	{
		TArray<FAWeekItemEntry> Items = GenerateRandomLoot();
		for (const FAWeekItemEntry& ItemEntry : Items)
		{
			UAWeekItemBase* NewItem = UAWeekItemBase::CreateFromData(
				ItemEntry.ItemData,
				ItemEntry.Quantity,
				InventoryComponent
			);

			if (NewItem)
			{
				InventoryComponent->HandleAddItem(NewItem);
			}
		}
	}
}

void AAWeekChest::BeginPlay()
{
	Super::BeginPlay();
	
	ChestExitSphere->OnComponentEndOverlap.AddDynamic(this, &AAWeekChest::OnChestRadiusExit);
	InitializeChest();
}

void AAWeekChest::BeginFocus()
{
	if (ChestMesh)
	{
		ChestMesh->SetRenderCustomDepth(true);
	}
}

void AAWeekChest::EndFocus()
{
	if (ChestMesh)
	{
		ChestMesh->SetRenderCustomDepth(false);
	}
}

void AAWeekChest::BeginInteract()
{
}

void AAWeekChest::EndInteract()
{

}

void AAWeekChest::Interact(TObjectPtr<AAWeekPlayerCharacter> Character)
{
	PlayerCharacter = Character;
	PlayerCharacter->ToggleChestInventory(InventoryComponent);
}

void AAWeekChest::OnChestRadiusExit(UPrimitiveComponent* OverlappedComponent,
									AActor* OtherActor,
									UPrimitiveComponent* OtherComp,
									int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AAWeekPlayerCharacter::StaticClass()))
	{
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->CloseChestInventory();
			PlayerCharacter = nullptr;
		}
	}
}

TArray<FAWeekItemEntry> AAWeekChest::GenerateRandomLoot()
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
	