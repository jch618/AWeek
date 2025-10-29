// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include"BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void ABaseEnemyAIController::Initialize()
{
    PerceptionComp = FindComponentByClass<UAIPerceptionComponent>();
    if (PerceptionComp)
    {
        // Get Sight Sense ID 
        FAISenseID SightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());

        // Get SightConfig
        const UAISenseConfig* BaseConfig = PerceptionComp->GetSenseConfig(SightSenseID);
        SightConfig = const_cast<UAISenseConfig_Sight*>(Cast<UAISenseConfig_Sight>(BaseConfig));
    }

    RunBehaviorTree(BTAsset);
    UseBlackboard(BBAsset, BBComponent);
}


void ABaseEnemyAIController::SetBlackboardValues()
{
	if (BBComponent == nullptr)
		return;
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	BBComponent->SetValueAsObject(AttackTargetKeyName, PlayerCharacter);
}

void ABaseEnemyAIController::ChangeAISightRange(float SightRadius, float LoseSightRadius, 
    float VisionAngleDegrees, float MaxAgeSeconds)
{
    SightConfig->SightRadius = SightRadius;
    SightConfig->LoseSightRadius = LoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = VisionAngleDegrees;
    SightConfig->SetMaxAge(MaxAgeSeconds);

    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->RequestStimuliListenerUpdate();
}

