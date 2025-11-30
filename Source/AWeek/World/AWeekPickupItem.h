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
	AAWeekPickupItem();

	void InitializePickupItem(const int32 InQuantity);
	void InitializeDrop(TObjectPtr<UAWeekItemBase> ItemToDrop, const int32 InQuantity);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter) override;
	virtual FORCEINLINE const FAWeekInteractableData& GetInteractableData() const override { return InteractableData; }
	
	FORCEINLINE TObjectPtr<UAWeekItemBase> GetItemData() { return Item; }

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	TObjectPtr<UAWeekItemBase> Item;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	FAWeekInteractableData InteractableData;
	
	void UpdateInteractableData();
	void TakePickup(const TObjectPtr<AAWeekPlayerCharacter> Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
