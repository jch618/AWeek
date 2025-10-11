
#include "AWeek/World/AWeekPickupItem.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
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
	if (!ItemRowHandle.IsNull())
	{
		const FAWeekItemData* ItemData = ItemRowHandle.GetRow<FAWeekItemData>(ItemRowHandle.RowName.ToString());

		ItemReference = NewObject<UAWeekItemBase>(this, UAWeekItemBase::StaticClass());
		ItemReference->InitializeItem(*ItemData, InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void AAWeekPickupItem::InitializeDrop(TObjectPtr<UAWeekItemBase> ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);

	UpdateInteractableData();
}

void AAWeekPickupItem::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EAWeekInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
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
		if (ItemReference)
		{
			if (TObjectPtr<UAWeekInventoryComponent> PlayerInventory = Taker->GetPlayerInventoryComponent())
			{
				const FAWeekItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

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
