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

void UAWeekInteractionWidget::UpdateWidget(const TObjectPtr<FInteractableData> InteractableData) const
{
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if (InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"),
				InteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;

	case EInteractableType::NonplayerCharacter:
		break;

	case EInteractableType::Device:
		break;

	case EInteractableType::Toggle:
		break;

	case EInteractableType::Chest:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		break;

	default:
		break;
	}

	ActionText->SetText(InteractableData->Action);
	NameText->SetText(InteractableData->Name);
}

float UAWeekInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}

