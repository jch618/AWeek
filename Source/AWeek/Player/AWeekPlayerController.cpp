// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerController.h"
#include "../UI/AWeekUserWidget.h"
#include "AWeek/UI/AWeekInventoryMainPanel.h"
#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"

#include "CommonUIExtensions.h"

AAWeekPlayerController::AAWeekPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bIsChestOpen(false)
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

	check(InventoryMainPanelClass);
	check(InteractionWidgetClass);

	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetLocalPlayer(),
			FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
		InventoryMainPanelWidget->InitializeInventoryMainPanel();
		InventoryMainPanelWidget->DeactivateWidget();
	}
}

// =====================================================
// INVENTORY SYSTEM
// =====================================================

void AAWeekPlayerController::ShowMainPanel()
{
	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetLocalPlayer(),
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
	}
}
void AAWeekPlayerController::HideMainPanel()
{
	if (InventoryMainPanelWidget)
	{
		InventoryMainPanelWidget->DeactivateWidget();
	}
}

void AAWeekPlayerController::ToggleMainPanel()
{
	if (!IsValid(InventoryMainPanelWidget) || !InventoryMainPanelWidget->IsActivated())
	{
		ShowMainPanel();
		SetShowMouseCursor(true);
	}
	else
	{
		if (bIsChestOpen)
		{
			DeactivateChestInventory();
		}
		HideMainPanel();
		SetShowMouseCursor(false);
	}
}

void AAWeekPlayerController::ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
{
	if (!bIsChestOpen)
	{
		if (!IsValid(InventoryMainPanelWidget) || !InventoryMainPanelWidget->IsActivated())
		{
			ShowMainPanel();
			SetShowMouseCursor(true);
		}
		ActivateChestInventory(ChestInventory);
	}
	else
	{
		DeactivateChestInventory();
		HideMainPanel();
		SetShowMouseCursor(false);
	}
}


void AAWeekPlayerController::ShowCrosshair()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAWeekPlayerController::HideCrosshair()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AAWeekPlayerController::ShowInteractionWidget()
{
	if (InteractionWidgetClass)
	{
		InteractionWidget = Cast<UAWeekInteractionWidget, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetLocalPlayer(),
				FGameplayTag::RequestGameplayTag("UI.Layer.Game"), InteractionWidgetClass));
	}
}
void AAWeekPlayerController::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->DeactivateWidget();
	}
}
void AAWeekPlayerController::UpdateInteractionWidget(const FAWeekInteractableData* InteractableData)
{
	if (InteractionWidget == nullptr || !InteractionWidget->IsActivated())
	{
		if (InteractionWidgetClass)
		{
			ShowInteractionWidget();
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void AAWeekPlayerController::ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
{
	bIsChestOpen = true;
	InventoryMainPanelWidget->ActivateChestInventory(ChestInventory);
}

void AAWeekPlayerController::DeactivateChestInventory()
{
	bIsChestOpen = false;
	InventoryMainPanelWidget->DeActivateChestInventory();
}
