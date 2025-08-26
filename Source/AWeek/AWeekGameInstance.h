// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AWeekGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UAWeekGameInstance();

public:
	virtual void Init();
	virtual void Shutdown();
};
