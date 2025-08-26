// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameModeBase.h"

#include "AWeekExperienceManagerComponent.h"
#include "AWeekGameStateBase.h"
#include "AWeekWorldSettings.h"
#include "AWeek/Character/AWeekCharacter.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeek/Player/AWeekPlayerState.h"
#include "AWeek/UI/AWeekHUD.h"

AAWeekGameModeBase::AAWeekGameModeBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	GameStateClass = AAWeekGameStateBase::StaticClass();
	PlayerControllerClass = AAWeekPlayerController::StaticClass();
	PlayerStateClass = AAWeekPlayerState::StaticClass();
	DefaultPawnClass = AAWeekPlayerCharacter::StaticClass();
	HUDClass = AAWeekHUD::StaticClass();
}

void AAWeekGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void AAWeekGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	if (AWorldSettings* WorldSettings = GetWorldSettings())
	{
		if (const AAWeekWorldSettings* AwWorldSettings = Cast<AAWeekWorldSettings>(WorldSettings))
		{
			const FPrimaryAssetId PrimaryAssetId = AwWorldSettings->GetDefaultGameplayExperience();
			OnMatchAssignmentGiven(PrimaryAssetId);
		}
	}
}

void AAWeekGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	check(ExperienceId.IsValid())
	UAWeekExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UAWeekExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}
