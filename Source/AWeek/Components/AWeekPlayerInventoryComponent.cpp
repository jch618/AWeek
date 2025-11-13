// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Components/AWeekPlayerInventoryComponent.h"

#include "IDetailTreeNode.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Player/Weapon/AWeekWeaponComponent.h"

UAWeekPlayerInventoryComponent::UAWeekPlayerInventoryComponent()
	: HotBarInventorySize(9), HotBarCurrentIndex(0), HotBarStartIndex(0), TrashCanSlotIndex(0)
{
	
}

void UAWeekPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	HotBarStartIndex = 0;
	// Trash can slot
	TrashCanSlotIndex = InventoryContents.Num() - 1;
}

void UAWeekPlayerInventoryComponent::UseSelectedItemPrimary(const TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter)
{
	const FAWeekInventorySlotData& SlotData = InventoryContents[HotBarCurrentIndex];
	if (!SlotData.bIsEmpty)
	{
		SlotData.Item->UsePrimary(PlayerCharacter);
	}
}

void UAWeekPlayerInventoryComponent::UseSelectedItemSecondary(const TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter)
{
	const FAWeekInventorySlotData& SlotData = InventoryContents[HotBarCurrentIndex];
	if (!SlotData.bIsEmpty)
	{
		SlotData.Item->UseSecondary(PlayerCharacter);
	}
}


void UAWeekPlayerInventoryComponent::SelectItemInHotBar(const int32 KeyboardNum)
{
	PreviousIndex = HotBarCurrentIndex;
	HotBarCurrentIndex = KeyboardNum % HotBarInventorySize;
	OnHotbarSelectionChanged.Broadcast(HotBarPreviousIndex, HotBarCurrentIndex);
}

void UAWeekPlayerInventoryComponent::SelectNextItemInHotBar()
{
	HotBarPreviousIndex = HotBarCurrentIndex;
	HotBarCurrentIndex = (HotBarCurrentIndex + 1) % HotBarInventorySize;
	OnHotbarSelectionChanged.Broadcast(HotBarPreviousIndex, HotBarCurrentIndex);
}

void UAWeekPlayerInventoryComponent::SelectPreviousItemInHotBar()
{
	HotBarPreviousIndex = HotBarCurrentIndex;
	HotBarCurrentIndex = (HotBarCurrentIndex + HotBarInventorySize - 1) % HotBarInventorySize;
	OnHotbarSelectionChanged.Broadcast(HotBarPreviousIndex, HotBarCurrentIndex);
}

void UAWeekPlayerInventoryComponent::SelectCurrentItemInHotBar()
{
	const FAWeekInventorySlotData& SlotData = InventoryContents[HotBarCurrentIndex];
	if (SlotData.Item && SlotData.Item->GetItemType() == EAWeekItemType::Weapon)
	{
		if (AAWeekPlayerCharacter* PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwner()))
		{
			const FName WeaponID = SlotData.Item->GetItemData().WeaponID; 
			PlayerCharacter->GetWeaponComponent()->ChangeWeapon(WeaponID);
			PlayerCharacter->SetAnimInstance(WeaponID);
		}
	}
}

void UAWeekPlayerInventoryComponent::SetTrashCanSlot(const TObjectPtr<UAWeekItemBase> InItem)
{
	PlaceItemAt(InItem, TrashCanSlotIndex);
}

void UAWeekPlayerInventoryComponent::ClearTrashCanSlot()
{
	if (IsValidItemSlotIndex(TrashCanSlotIndex))
	{
		ClearItemSlot(InventoryContents[TrashCanSlotIndex]);
	}
}
