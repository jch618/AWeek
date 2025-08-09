// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekHUD.h"

#include "Components/GameFrameworkComponentManager.h"

AAWeekHUD::AAWeekHUD(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void AAWeekHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void AAWeekHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void AAWeekHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}
