// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/World/AWeekCraftingTable.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "Components/SphereComponent.h"

AAWeekCraftingTable::AAWeekCraftingTable() : CraftingTableExitRadius(225.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CraftingTableMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	SetRootComponent(CraftingTableMesh);

	CraftingTableExitSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	CraftingTableExitSphere->SetupAttachment(RootComponent);
	CraftingTableExitSphere->InitSphereRadius(CraftingTableExitRadius);
}

void AAWeekCraftingTable::BeginFocus()
{
	if (CraftingTableMesh)
	{
		CraftingTableMesh->SetRenderCustomDepth(true);
	}
}

void AAWeekCraftingTable::EndFocus()
{
	if (CraftingTableMesh)
	{
		CraftingTableMesh->SetRenderCustomDepth(false);
	}
}

void AAWeekCraftingTable::BeginInteract()
{
}

void AAWeekCraftingTable::EndInteract()
{
}

void AAWeekCraftingTable::Interact(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter)
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->ToggleCraftingPanel();
	}
}

void AAWeekCraftingTable::OnCraftingTableRadiusExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AAWeekPlayerCharacter::StaticClass()))
	{
		AAWeekPlayerCharacter* PlayerCharacter = Cast<AAWeekPlayerCharacter>(OtherActor);
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->CloseCraftingPanel();
		}
	}
}

void AAWeekCraftingTable::BeginPlay()
{
	Super::BeginPlay();
	
}
