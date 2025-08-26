// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameInstance.h"
#include "UI/AWeekUIManager.h"

UAWeekGameInstance::UAWeekGameInstance()
{
}

void UAWeekGameInstance::Init()
{
	Super::Init();
}

void UAWeekGameInstance::Shutdown()
{
	Super::Shutdown();

	CAWeekUIManager::DestroyInst();
}
