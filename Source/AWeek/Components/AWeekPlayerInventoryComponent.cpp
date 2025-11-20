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
	if (!SlotData.IsEmpty())
	{
		SlotData.Item->UsePrimary(PlayerCharacter);
	}
}

void UAWeekPlayerInventoryComponent::UseSelectedItemSecondary(const TObjectPtr<AAWeekPlayerCharacter> PlayerCharacter)
{
	const FAWeekInventorySlotData& SlotData = InventoryContents[HotBarCurrentIndex];
	if (!SlotData.IsEmpty())
	{
		SlotData.Item->UseSecondary(PlayerCharacter);
	}
}


void UAWeekPlayerInventoryComponent::SelectItemInHotBar(const int32 InHotBarIndex)
{
	if (HotBarCurrentIndex != InHotBarIndex)
	{
		HotBarPreviousIndex = HotBarCurrentIndex;
		HotBarCurrentIndex = InHotBarIndex % HotBarInventorySize;
		SelectCurrentItemInHotBar();
		OnHotbarSelectionChanged.Broadcast(HotBarPreviousIndex, HotBarCurrentIndex);
	}
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
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	
	if (AAWeekPlayerCharacter* PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwner()))
	{
		const FAWeekInventorySlotData& SlotData = InventoryContents[HotBarCurrentIndex];
		if (SlotData.Item && SlotData.Item->GetItemType() == EAWeekItemType::Weapon)
		{
			const FName WeaponID = SlotData.Item->GetItemData().WeaponID; 
			PlayerCharacter->GetWeaponComponent()->ChangeWeapon(WeaponID);
			PlayerCharacter->SetAnimInstance(WeaponID);
		}
		else
		{
			PlayerCharacter->GetWeaponComponent()->ChangeWeapon("Default");
			PlayerCharacter->SetAnimInstance("Default");
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

int UAWeekPlayerInventoryComponent::AddNewItem(UAWeekItemBase* Item, const int32 AmountToAdd, int32 InTargetIndex)
{
	int32 TargetIndex = Super::AddNewItem(Item, AmountToAdd, InTargetIndex);
	if (IsHotBarSlotIndex(TargetIndex) && TargetIndex == HotBarCurrentIndex)
	{
		SelectCurrentItemInHotBar();
	}
	return TargetIndex;
}

void UAWeekPlayerInventoryComponent::ClearItemSlot(FAWeekInventorySlotData& ItemSlotToRemove)
{
	Super::ClearItemSlot(ItemSlotToRemove);

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (IsHotBarSlotIndex(ItemSlotToRemove.SlotIndex) && ItemSlotToRemove.SlotIndex == HotBarCurrentIndex)
	{
		SelectCurrentItemInHotBar();
	}
}
