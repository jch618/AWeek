// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AWeek/Interfaces/AWeekInteractionInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWeekChest.generated.h"

struct FAWeekItemEntry;
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

protected:
	void BeginPlay() override;

public:
	void BeginFocus() override;
	void EndFocus() override;
	void BeginInteract() override;
	void EndInteract() override;
	void Interact(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter) override;
	FORCEINLINE virtual const FAWeekInteractableData& GetInteractableData() const override { return InteractableData; }
	FORCEINLINE UAWeekInventoryComponent* GetInventoryComponent() const { return InventoryComponent;}
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Chest")
	FAWeekInteractableData InteractableData;
	
	UPROPERTY(VisibleAnywhere, Category = "Chest")
	TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter;

	UPROPERTY(VisibleAnywhere, Category = "Chest")
	TObjectPtr<UAWeekInventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Chest")
	TObjectPtr<UStaticMeshComponent> ChestMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Chest")
	TObjectPtr<USphereComponent> ChestExitSphere;

	UPROPERTY(EditAnywhere, Category = "Chest")
	float ChestExitRadius;
	
	
	UFUNCTION()
	void OnChestRadiusExit(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
