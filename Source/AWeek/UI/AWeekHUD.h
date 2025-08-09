// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AWeekHUD.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API AAWeekHUD : public AHUD
{
	GENERATED_BODY()
public:
	AAWeekHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
