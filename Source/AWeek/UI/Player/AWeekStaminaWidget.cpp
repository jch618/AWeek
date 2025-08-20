// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekStaminaWidget.h"

UAWeekStaminaWidget::UAWeekStaminaWidget(const FObjectInitializer& ObjectInitializer) : UAWeekUserWidget(ObjectInitializer)
{
	mWidgetName = TEXT("StaminaWidget");
}

void UAWeekStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HideWidget();
	mAnim = GetWidgetAnimation(TEXT("StaminaDisappear"));
	mProgress = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaProgress")));
	mAnimFinishDelegate.BindDynamic(this, &UAWeekStaminaWidget::HideWidget);

	if (mAnim)
	{
		BindToAnimationFinished(mAnim, mAnimFinishDelegate);
	}
}

void UAWeekStaminaWidget::PlayDisappearAnimation()
{
	PlayWidgetAnimation(TEXT("StaminaDisappear"), 0.f, 1.f, true, true);
}

void UAWeekStaminaWidget::AfterUseStamina(float CurrentStamina)
{
	SetVisibility(ESlateVisibility::Visible);

	if (IsAnimationPlaying(mAnim))
		StopAnimation(mAnim);

	UpdateProgress(CurrentStamina);
}
