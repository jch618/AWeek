// Fill out your copyright notice in the Description page of Project Settings.

#include "AWeekStaminaComponent.h"

UAWeekStaminaComponent::UAWeekStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAWeekStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Stamina Recovery
	if (bRecovery && mStamina < mMaxStamina)
	{
		ChangeStamina(mRecoveryRate * DeltaTime);
	}
}

bool UAWeekStaminaComponent::UseStamina(EStaminaUseType StaminaUseType)
{
	float Usage = 0;
	float DeltaTime = GetWorld()->DeltaTimeSeconds;

	switch (StaminaUseType)
	{
	case EStaminaUseType::Sprint:
		Usage = mSprintUsage * DeltaTime;
		break;
	case EStaminaUseType::Vault:
		Usage = mVaultUsage;
		break;
	case EStaminaUseType::Climb:
		Usage = mClimbUsage;
		break;
	}

	if (mStamina < Usage)
		return false;

	ChangeStamina(-Usage);
	bRecovery = false;

	GetOwner()->GetWorldTimerManager().ClearTimer(mRecoveryTimer);
	GetOwner()->GetWorldTimerManager().SetTimer(
		mRecoveryTimer,
		this,
		&UAWeekStaminaComponent::EnableStaminaRecovery,
		mTimeToRecovery,
		false
	);

	return true;
}