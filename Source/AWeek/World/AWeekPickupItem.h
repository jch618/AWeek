#pragma once

#include "AWeek/Interfaces/AWeekInteractionInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AWeekPickupItem.generated.h"

class UDataTable;
class UAWeekItemBase;
class AAWeekPlayerCharacter;

UCLASS()
class AWEEK_API AAWeekPickupItem : public AActor, public IAWeekInteractionInterface
{
	GENERATED_BODY()

public:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================


	//================================================================
	//	FUNCTIONS
	//================================================================
	AAWeekPickupItem();

	void InitializePickupItem(const int32 InQuantity);

	void InitializeDrop(TObjectPtr<UAWeekItemBase> ItemToDrop, const int32 InQuantity);

	FORCEINLINE TObjectPtr<UAWeekItemBase> GetItemData() { return ItemReference; }

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:
	//================================================================
	//	PROPERTIES & VARIABLES
	//================================================================

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	TObjectPtr<UAWeekItemBase> ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FAWeekInteractableData InstanceInteractableData;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FDataTableRowHandle ItemRowHandle;


	//================================================================
	//	FUNCTIONS
	//================================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter) override;
	void UpdateInteractableData();
	void TakePickup(const TObjectPtr<AAWeekPlayerCharacter> Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
