// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEventMessageSubsystem.h"
#include "AWeek/System/AWeekEventMessageInfo.h"
#include "Components/ActorComponent.h"
#include "AWeekStaminaComponent.generated.h"

UENUM()
enum class EStaminaUseType
{
	Sprint,
	Vault,
	Ledge,
	LedgeStart
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UAWeekStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAWeekStaminaComponent();

protected:
	UPROPERTY(EditAnywhere)
	float mMaxStamina = 100; // ������ 100

	UPROPERTY(VisibleAnywhere)
	float mStamina = 100; // ������ 100

	UPROPERTY(EditAnywhere)
	float mRecoveryRate = 20;

	UPROPERTY(EditAnywhere)
	float mTimeToRecovery = 2;

	FTimerHandle mRecoveryTimer;
	bool bRecovery = true;

protected:
	UPROPERTY(EditAnywhere)
	float mSprintUsage = 20; // �ʴ� �Ҹ�

	UPROPERTY(EditAnywhere)
	float mVaultUsage = 10;

	UPROPERTY(EditAnywhere)
	float mLedgeUsage = 10; // �ʴ� �Ҹ�

	UPROPERTY(EditAnywhere)
	float mLedgeStartUsage = 10;

protected:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ChangeStamina(float Amount)
	{
		mStamina = FMath::Min(mStamina + Amount, mMaxStamina);
		FStaminaChangedMessage Msg;
		Msg.Stamina = mStamina;
		Msg.Amount = Amount;
		UGameEventMessageSubsystem::Get(this).BroadcastMessage(
			FGameplayTag::RequestGameplayTag(FName("Event.StaminaChanged")),
			Msg
		);
	}

public:
	bool UseStamina(EStaminaUseType StaminaUseType);
	float GetStamina()
	{
		return mStamina;
	}
	void EnableStaminaRecovery()
	{
		bRecovery = true;
	}
};
