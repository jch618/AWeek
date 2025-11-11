// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_RegisterInput.h"

#include "EnhancedInputSubsystems.h"
#include "AWeek/AWeekAssetManager.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UGameFeatureAction_RegisterInput::OnGameFeatureRegistering()
{
	Super::OnGameFeatureRegistering();
	
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (const UWorld* World = Context.World())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (ULocalPlayer* LP = GI->GetFirstGamePlayer())
				{
					RegisterInputMappingContexts(LP);
				}
			}
		}
	}
}

void UGameFeatureAction_RegisterInput::RegisterInputMappingContexts(ULocalPlayer* LocalPlayer)
{
	UAWeekAssetManager& AssetManager = UAWeekAssetManager::Get();
		
	if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		if (UEnhancedInputUserSettings* Settings = EISubsystem->GetUserSettings())
		{
			for (const TObjectPtr<UInputMappingContext>& Entry : InputMappings)
			{
				Settings->RegisterInputMappingContext(Entry);
			}
		}
	}
}
