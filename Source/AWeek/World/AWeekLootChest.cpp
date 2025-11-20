// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/World/AWeekLootChest.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"

void AAWeekLootChest::AWeekLootChest()
{
	LifeSpan = 20.0f;

	ChestMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChestMesh->SetSimulatePhysics(false);
}

void AAWeekLootChest::BeginPlay()
{
	Super::BeginPlay();

	// TODO: fill inventory slot with items zombie dropped 

	GetWorldTimerManager().SetTimer(LifeSpanTimer,
		this,
		&AAWeekLootChest::DestoryOnTimerExpired,
		LifeSpan,
		false);
}

void AAWeekLootChest::DestoryOnTimerExpired()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->CloseChestInventory();
		PlayerCharacter = nullptr;
	}
	Destroy();
}
