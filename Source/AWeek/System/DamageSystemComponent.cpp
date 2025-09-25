

#include "DamageSystemComponent.h"

UDamageSystemComponent::UDamageSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float UDamageSystemComponent::GetCurrentHealth_Implementation() const
{
	return Health;
}

float UDamageSystemComponent::GetMaxHealth_Implementation() const
{
	return MaxHealth;
}

float UDamageSystemComponent::Heal_Implementation(float Amount)
{
	if (IsDead)
		return 0;

	float OldHealth = Health;
	Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
	
	return Health - OldHealth;
}

bool UDamageSystemComponent::GetIsDead_Implementation()
{
	return IsDead;
}

bool UDamageSystemComponent::TakeDamage_Implementation(FDamageInfo DamageInfo)
{
	//No Damage
	if (IsDead || (IsInvincible && !DamageInfo.ShouldDamageInvincible))
		return false;
	
	//BlockDamage
	if (IsBlocking && DamageInfo.CanBeBlocked)
	{
		OnBlocked.Broadcast(DamageInfo.CanBeParried);
		return false;
	}
	
	//Do Damage
	Health = FMath::Clamp(Health - DamageInfo.Amount, 0.f, MaxHealth);
	if (Health <= 0)
	{
		IsDead = true;
		OnDeath.Broadcast();
		return true;
	}
	//DamageResponse
	if (IsInterruptable || DamageInfo.ShouldForceInterrupt)
		OnDamageResponse.Broadcast(DamageInfo.DamageResponse);

	return true;
}

bool UDamageSystemComponent::ReserveAttackToken_Implementation(int Amount = 1)
{
	if (AttackTokenCount >= Amount)
	{
		AttackTokenCount -= Amount;
		return true;
	}
	return false;
}

void UDamageSystemComponent::ReturnAttackToken_Implementation(int Amount = 1)
{
	AttackTokenCount += Amount;
}

#if WITH_EDITOR
void UDamageSystemComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UDamageSystemComponent, MaxHealth))
	{
	}
		Health = MaxHealth; 
}
#endif
// Called when the game starts
void UDamageSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}