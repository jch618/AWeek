// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include"BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void ABaseEnemyAIController::Initialize()
{
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


