// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AWeek/Interfaces/AWeekInteractionInterface.h"

void UAWeekInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UAWeekInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}

void UAWeekInteractionWidget::UpdateWidget(const FAWeekInteractableData* FInteractableData) const
{
	switch (FInteractableData->InteractableType)
	{
	case EAWeekInteractableType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if (FInteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"),
				FInteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;

	case EAWeekInteractableType::NonplayerCharacter:
		break;

	case EAWeekInteractableType::Device:
		break;

	case EAWeekInteractableType::Toggle:
		break;

	case EAWeekInteractableType::Chest:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		break;

	default:
		break;
	}

	ActionText->SetText(FInteractableData->Action);
	NameText->SetText(FInteractableData->Name);
}

float UAWeekInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}

