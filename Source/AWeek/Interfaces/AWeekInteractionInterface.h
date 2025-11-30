// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AWeekInteractionInterface.generated.h"

class AAWeekPlayerCharacter;

UENUM()
enum class EAWeekInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	CraftingTable UMETA(DisplayName = "CraftingTable"),
	Chest UMETA(DisplayName = "Chest"),
	Device UMETA(DisplayName = "Device")
};
USTRUCT()
struct FAWeekInteractableData
{
	GENERATED_BODY()

	FAWeekInteractableData() :
		InteractableType(EAWeekInteractableType::Pickup),
		Name(FText::GetEmpty()),
		Action(FText::GetEmpty()),
		Quantity(0),
		InteractionDuration(0.0f)
	{ }
	
	UPROPERTY(EditDefaultsOnly)
	EAWeekInteractableType InteractableType;

	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	FText Action;

	// used only for pickups
	UPROPERTY(EditDefaultsOnly)
	int8 Quantity;

	// used for things like valves, doors, etc. that require an interaction timer
	UPROPERTY(EditDefaultsOnly)
	float InteractionDuration;

	bool IsEmpty() const { return Name.IsEmpty(); }
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAWeekInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class AWEEK_API IAWeekInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual void Interact(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter);
	
	virtual const FAWeekInteractableData& GetInteractableData() const = 0;
};
