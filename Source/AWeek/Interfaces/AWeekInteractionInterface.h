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
	NonplayerCharacter UMETA(DisplayName = "NonplayerCharacter"),
	Device UMETA(DisplayName = "Device"),
	Toggle UMETA(DisplayName = "Toggle"),
	Chest UMETA(DisplayName = "Chest")
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
	{

	}

	UPROPERTY(EditInstanceOnly)
	EAWeekInteractableType InteractableType;

	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	// used only for pickups
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	// used for things like valves, doors, etc. that require an interaction timer
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
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

	FAWeekInteractableData InteractableData;
};
