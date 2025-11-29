// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEventMessageSubsystem.h"
#include "AWeek/System/AWeekEventMessageInfo.h"
#include "Components/ActorComponent.h"
#include "AWeekHungerComponent.generated.h"

UENUM()
enum class EHungerState
{
	Healthy,
	Hungry,
	Starving,
	Fainting
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UAWeekHungerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAWeekHungerComponent();

protected:
	EHungerState HungerState = EHungerState::Healthy;

	UPROPERTY(VisibleAnywhere)
	float Hunger = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHunger = 100;

	// ����� ���¹̳��� 1/N�踸ŭ ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaAffected = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UsageRate = 120; // 600�ʵڿ� ��� ��� �Ҹ�

	FGameEventMessageListenerHandle StaminaChangedHandle;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ChangeHunger(float Amount)
	{
		Hunger = FMath::Min(Hunger + Amount, MaxHunger);

		FHungerChangedMessage Msg;
		Msg.Hunger = Hunger;
		Msg.MaxHunger = MaxHunger;
		Msg.Amount = Amount;
		UGameEventMessageSubsystem::Get(this).BroadcastMessage(
			FGameplayTag::RequestGameplayTag(FName("Event.HungerChanged")),
			Msg
		);
	}

public:
	bool IsOnHungerState(EHungerState CheckState)
	{
		EHungerState CurrentState = EHungerState::Healthy;
		if (Hunger > 50)
		{
			CurrentState = EHungerState::Healthy;
		}
		else if (Hunger <= 50 && Hunger > 30)
		{
			CurrentState = EHungerState::Hungry;
	
		}
		else if (Hunger <= 30 && Hunger > 5)
		{
			CurrentState = EHungerState::Starving;
		}
		else
		{
			CurrentState = EHungerState::Fainting;
		}

		return CheckState == CurrentState;
	}
};
