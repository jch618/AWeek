#include "AWeek/Items/AWeekConsumableItem.h"

#include "AWeek/Character/AWeekPlayerCharacter.h"

bool UAWeekConsumableItem::UseSecondary(TObjectPtr<AAWeekPlayerCharacter> Character)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	const FAWeekConsumableData& ConsumableStats = ItemData.ConsumableData;
	Character->AddHealth(ConsumableStats.HealthDelta);
	Character->AddStamina(ConsumableStats.StaminaDelta);
	Character->AddHunger(ConsumableStats.HungerDelta);
	return true;
}
