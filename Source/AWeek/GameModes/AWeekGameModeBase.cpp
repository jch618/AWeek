// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameModeBase.h"

#include "AWeekGameStateBase.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeek/Player/AWeekPlayerState.h"
#include "AWeek/UI/AWeekHUD.h"

AAWeekGameModeBase::AAWeekGameModeBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	GameStateClass = AAWeekGameStateBase::StaticClass();
	PlayerControllerClass = AAWeekPlayerController::StaticClass();
	PlayerStateClass = AAWeekPlayerState::StaticClass();
	DefaultPawnClass = AAWeekPlayerCharacter::StaticClass();
	HUDClass = AAWeekHUD::StaticClass();
}
