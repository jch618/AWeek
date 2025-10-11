// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/MainWidget/ButtonPanelWidget.h"

#include "CommonUIExtensions.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"

void UButtonPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ensure(Button_0))
	{
		Button_0->OnClicked.RemoveAll(this);
		Button_0->OnClicked.AddDynamic(this, &UButtonPanelWidget::HandleToggleClicked);
	}
}

void UButtonPanelWidget::HandleToggleClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("UI Test Start!!!"));

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAWeekPlayerController* UIPC = Cast<AAWeekPlayerController>(PC);

	if (!IsValid(TestPanelToButton) || !(TestPanelToButton->IsActivated()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UI TestActicatableWidget is false!!"));
		if (TestPanelToButtonClass)
		{
			TestPanelToButton = CreateWidget<UTestPanelToButton>(GetWorld(), TestPanelToButtonClass);
			FloatingLayer->AddChildToOverlay(TestPanelToButton);
			TestPanelToButton->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			TestPanelToButton->ActivateWidget();
			/*//TestActiveatableWidget = Cast<
			TestPanelToButton = Cast<UTestPanelToButton, UCommonActivatableWidget>
			(UCommonUIExtensions::PushContentToLayer_ForPlayer(UIPC->GetLocalPlayer(),
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), TestPanelToButtonClass))
				;*/
		}else{UE_LOG(LogTemp, Warning, TEXT("UI Test Failed!!!"));}

		UIPC->SetShowMouseCursor(true);
	}
}
