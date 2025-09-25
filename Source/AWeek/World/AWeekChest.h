// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AWeek/Interfaces/AWeekInteractionInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWeekChest.generated.h"

class AAWeekPlayerCharacter;
struct FAWeekInventorySlotData;
class UAWeekInventoryComponent;
class USphereComponent;

UCLASS()
class AWEEK_API AAWeekChest : public AActor, public IAWeekInteractionInterface
{
	GENERATED_BODY()

public:
	AAWeekChest();

	void BeginFocus() override;
	void EndFocus() override;
	void BeginInteract() override;
	void EndInteract() override;
	void Interact(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter) override;

protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================
	UPROPERTY(VisibleAnywhere, Category = "Chest")
	TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Chest")
	TObjectPtr<UAWeekInventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Chest")
	TObjectPtr<UStaticMeshComponent> ChestMesh;

	UPROPERTY()
	TObjectPtr<USphereComponent> ChestExitSphere;

	UPROPERTY(EditAnywhere, Category = "Chest | Initialization")
	float ChestExitRadius;

	//================================================================
	//	FUNCTIONS
	//================================================================
	void BeginPlay() override;
	
	UFUNCTION()
	void OnChestRadiusExit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:


	//================================================================
	//	FUNCTIONS
	//================================================================

};
