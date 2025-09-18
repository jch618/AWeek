// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekMainMenuStateComponent.h"

#include "CommonUIExtensions.h"

void UAWeekMainMenuStateComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(!MainMenuClass.IsNull()))
	{
		if (UWorld* World = GetWorld())
		{
			if (ULocalPlayer* Player = World->GetFirstLocalPlayerFromController())
			{
				UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(Player,
					FGameplayTag::RequestGameplayTag(TEXT("UI.Layer.Menu")), MainMenuClass);
			}
		}
	}
}
