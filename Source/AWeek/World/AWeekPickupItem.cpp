
#include "AWeek/World/AWeekPickupItem.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Components/AWeekPlayerInventoryComponent.h"
#include "AWeek/Components/AWeekInventoryComponent.h"

AAWeekPickupItem::AAWeekPickupItem()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);
}

void AAWeekPickupItem::BeginPlay()
{
	Super::BeginPlay();

	InitializePickupItem(ItemQuantity);
}

void AAWeekPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAWeekPickupItem::InitializePickupItem(const int32 InQuantity)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (!ItemRowHandle.IsNull())
	{
		Item = UAWeekItemBase::CreateFromRowHandle(ItemRowHandle, InQuantity, GetWorld());
		PickupMesh->SetStaticMesh(Item->GetAssetData().Mesh);

		UpdateInteractableData();
	}
}

void AAWeekPickupItem::InitializeDrop(TObjectPtr<UAWeekItemBase> ItemToDrop, const int32 InQuantity)
{
	Item = ItemToDrop;
	InQuantity <= 0 ? Item->SetQuantity(1) : Item->SetQuantity(InQuantity);
	// Item->GetNumericData().Weight = ItemToDrop->GetItemSingleWeight();
	PickupMesh->SetStaticMesh(ItemToDrop->GetAssetData().Mesh);

	UpdateInteractableData();
}

void AAWeekPickupItem::UpdateInteractableData()
{
	InteractableData.InteractableType = EAWeekInteractableType::Pickup;
	InteractableData.Action = Item->GetTextData().InteractionText;
	InteractableData.Name = Item->GetTextData().Name;
	InteractableData.Quantity = Item->GetQuantity();
}

void AAWeekPickupItem::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void AAWeekPickupItem::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void AAWeekPickupItem::Interact(TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void AAWeekPickupItem::TakePickup(const TObjectPtr<AAWeekPlayerCharacter> Taker)
{
	if (!IsPendingKillPending())
	{
		if (Item)
		{
			if (TObjectPtr<UAWeekPlayerInventoryComponent> PlayerInventory = Taker->GetPlayerInventoryComponent())
			{
				// UE_LOG(LogTemp, Warning, TEXT("%s: ID: %s, Weight: %f"), *FString(__FUNCTION__), *Item->GetID().ToString(), Item->GetNumericData().Weight);
				const FAWeekItemAddResult AddResult = PlayerInventory->HandleAddItem(Item);

				switch (AddResult.OperationResult)
				{
				case EAWeekItemAddResult::IAR_NoItemAdded:
					break;
				case EAWeekItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EAWeekItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player Inventory component is null!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"));
		}
	}

}

#if WITH_EDITOR
void AAWeekPickupItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		if (!ItemRowHandle.IsNull())
		{
			if (const FAWeekItemData* ItemData = ItemRowHandle.GetRow<FAWeekItemData>(ItemRowHandle.RowName.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}
#endif
