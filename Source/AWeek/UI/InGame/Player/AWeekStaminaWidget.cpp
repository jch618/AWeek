// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekStaminaWidget.h"
#include "Components/WidgetComponent.h"
#include "Animation/WidgetAnimation.h"
#include "AWeek/System/AWeekEventMessageInfo.h"

UAWeekStaminaWidget::UAWeekStaminaWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UAWeekStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*--------------INIT--------------*/
	HideWidget();
	Progress = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaProgress")));
	
	/*--------------ANIMATION--------------*/
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	for (auto& Anim : WidgetClass->Animations)
	{
		FString	Name = Anim->GetName();
		Name.ReplaceInline(TEXT("_INST"), TEXT(""), ESearchCase::IgnoreCase);

		if (Name == TEXT("FadeOutAnim"))
			FadeOutAnim = Anim;
	}

	if (FadeOutAnim)
	{
		FWidgetAnimationDynamicEvent AnimFinishDelegate;
		AnimFinishDelegate.BindDynamic(this, &UAWeekStaminaWidget::HideWidget);
		BindToAnimationFinished(FadeOutAnim, AnimFinishDelegate);
		UE_LOG(LogTemp, Warning, TEXT("OK"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation Not Found"));
	}

	/*--------------EVENTMSSAGE--------------*/
	StaminaChangedHandle = UGameEventMessageSubsystem::Get(this).RegisterListener<FStaminaChangedMessage>(
		FGameplayTag::RequestGameplayTag(FName("Event.StaminaChanged")),
		[this](FGameplayTag Channel, const FStaminaChangedMessage& Msg)
		{
			if (Msg.Amount < 0) 
				UpdateProgress(Msg.Stamina, true);
			else
				UpdateProgress(Msg.Stamina, false);
		}
	);
}

void UAWeekStaminaWidget::UpdateProgress(float Stamina, bool bDecrease)
{
	Progress->SetPercent(Stamina / 100);

	// ���¹̳ʰ� �پ��������� Ÿ�̸� �ʱ�ȭ
	if (bDecrease)
	{
		ShowWidget();

		if (IsAnimationPlaying(FadeOutAnim))
			StopAnimation(FadeOutAnim);

		GetWorld()->GetTimerManager().ClearTimer(FadeOutTimer);
		GetWorld()->GetTimerManager().SetTimer(
			FadeOutTimer,
			[this]()
			{
				PlayAnimation(FadeOutAnim, 0, 1, EUMGSequencePlayMode::Forward, 1, true);
			},
			TimeToFadeOut,
			false
		);
	}
}
