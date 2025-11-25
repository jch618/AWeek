// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Interfaces/AWeekInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "AWeekCraftingTable.generated.h"

class USphereComponent;

UCLASS()
class AWEEK_API AAWeekCraftingTable : public AActor, public IAWeekInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AAWeekCraftingTable();

protected:
	virtual void BeginPlay() override;

public:
	void BeginFocus() override;
	void EndFocus() override;
	void BeginInteract() override;
	void EndInteract() override;
	void Interact(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Crafting Table | Initialization")
	TObjectPtr<UStaticMeshComponent> CraftingTableMesh;

	UPROPERTY(EditAnywhere, Category = "Crafting Table | Initialization")
	TObjectPtr<USphereComponent> CraftingTableExitSphere;
	
	UPROPERTY(EditAnywhere, Category = "Crafting Table | Initialization")
	float CraftingTableExitRadius;

	UFUNCTION()
	void OnCraftingTableRadiusExit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Crafting Table")
	int32 CraftingTableLevel;
};
