// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Items/AWeekWeaponItem.h"

#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Player/Weapon/AWeekWeaponComponent.h"

/* called when select weapon in hotbar */
bool UAWeekWeaponItem::UsePrimary(TObjectPtr<AAWeekPlayerCharacter> Character)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(__FUNCTION__));
	Character->GetWeaponComponent()->ChangeWeapon(ItemData.WeaponID);
	return true;
}

bool UAWeekWeaponItem::UseSecondary(TObjectPtr<AAWeekPlayerCharacter> Character)
{
	return Super::UseSecondary(Character);
}
