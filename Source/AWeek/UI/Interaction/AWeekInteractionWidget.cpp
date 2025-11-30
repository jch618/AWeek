// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"

#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AWeek/Interfaces/AWeekInteractionInterface.h"
#include "AWeek/UI/AWeekGameUIManager.h"

UAWeekInteractionWidget::UAWeekInteractionWidget(): CurrentInteractionDuration(0.0f)
{
}

void UAWeekInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
	if (AAWeekPlayerCharacter* PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn()))
	{
		PlayerCharacter->OnInteractionTargetChanged.AddUObject(this, &UAWeekInteractionWidget::OnInteractionTargetChanged);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: PlayerCharacter is null!"), *FString(__FUNCTION__));
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UAWeekInteractionWidget::OnInteractionTargetChanged(const FAWeekInteractableData& InteractableData)
{
	if (!InteractableData.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Visible);
		UpdateWidget(InteractableData);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAWeekInteractionWidget::UpdateWidget(const FAWeekInteractableData& InteractableData) const
{
	if (InteractableData.IsEmpty())
	{
		return;
	}
	switch (InteractableData.InteractableType)
	{
	case EAWeekInteractableType::Pickup:
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		KeyPressText->SetText(FText::FromString("Press"));

		if (InteractableData.Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"),
				InteractableData.Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;

	case EAWeekInteractableType::CraftingTable:
	case EAWeekInteractableType::Chest:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		break;

	case EAWeekInteractableType::Device:
		break;
		
	default:
		break;
	}

	ActionText->SetText(InteractableData.Action);
	NameText->SetText(InteractableData.Name);
}

float UAWeekInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}

