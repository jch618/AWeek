// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekHungerComponent.h"

// Sets default values for this component's properties
UAWeekHungerComponent::UAWeekHungerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...


}


// Called when the game starts
void UAWeekHungerComponent::BeginPlay()
{
	Super::BeginPlay();
	StaminaChangedHandle = UGameEventMessageSubsystem::Get(this).RegisterListener<FStaminaChangedMessage>(
		FGameplayTag::RequestGameplayTag(FName("Event.StaminaChanged")),
		[this](FGameplayTag Channel, const FStaminaChangedMessage& Msg)
		{
			if (Msg.Amount < 0)
				ChangeHunger(Msg.Amount / StaminaAffected);
		}
	);
}

void UAWeekHungerComponent::OnRegister()
{
	Super::OnRegister();
}


// Called every frame
void UAWeekHungerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float Usage = (MaxHunger / UsageRate) * DeltaTime;
	if (Hunger - Usage >= 0)
		ChangeHunger(-Usage);
}

