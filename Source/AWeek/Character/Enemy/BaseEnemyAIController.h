// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API ABaseEnemyAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Initialize();
	UFUNCTION(BlueprintCallable)
	void SetBlackboardValues();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttackTargetKeyName = "AttackTarget";

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BTAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackboardData* BBAsset;
	UPROPERTY(BlueprintReadOnly)
	UBlackboardComponent* BBComponent;
	UFUNCTION(BlueprintCallable)
	void ChangeAISightRange(float SightRadius, float LoseSightRadius, float VisionAngleDegrees, float MaxAgeSeconds);
};

