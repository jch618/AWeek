// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerController.h"
#include "../UI/AWeekUserWidget.h"

AAWeekPlayerController::AAWeekPlayerController(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

void AAWeekPlayerController::BeginPlay()
{
	Super::BeginPlay();

	mMainWidget = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Main.UI_Main_C'"));

	if (IsValid(mMainWidget))
	{
		UUserWidget* MainWidget = CreateWidget<UUserWidget>(this,
			mMainWidget);

		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}
}
