// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/MainWidget/MainWidgetButton.h"


void UMainWidgetButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	// 초기값 적용 (디자인 타임에서도 미리 보이게)
	if (Text)
	{
		Text->SetText(InitialText);
	}
	if (Icon)
	{
		if (InitialIcon)
		{
			Icon->SetBrushFromTexture(InitialIcon, /*bMatchSize=*/true);
			Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			Icon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMainWidgetButton::SetButtonText(const FText& InText)
{
	InitialText = InText;
	if (Text) Text->SetText(InText);
}

void UMainWidgetButton::SetButtonIcon(UTexture2D* InIcon)
{
	InitialIcon = InIcon;
	if (!Icon) return;

	if (InIcon)
	{
		Icon->SetBrushFromTexture(InIcon, /*bMatchSize=*/true);
		Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		Icon->SetVisibility(ESlateVisibility::Collapsed);
	}
}
