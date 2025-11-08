// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/World/AWeekChest.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekInventoryComponent.h"

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

void AAWeekChest::BeginPlay()
{
	Super::BeginPlay();
	
	ChestExitSphere->OnComponentEndOverlap.AddDynamic(this, &AAWeekChest::OnChestRadiusExit);
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
	if (InventoryComponent->IsLinkedToInventoryPanel())
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
}
