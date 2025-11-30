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

void UAWeekStaminaWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (GetVisibility() == ESlateVisibility::Hidden || GetVisibility() == ESlateVisibility::Collapsed)
	{
		return;
	}

	if (!bStartFade)
	{
		mCurrentDelayTime += DeltaTime;

		if (mCurrentDelayTime >= TimeToFadeOut)
		{
			bStartFade = true;
		}
	}
	else
	{
		float FadeSpeed = DeltaTime / FadeDuration;

		mCurrentFadeOpacity -= FadeSpeed;

		if (mCurrentFadeOpacity <= 0.0f)
		{
			mCurrentFadeOpacity = 0.0f;
			SetVisibility(ESlateVisibility::Hidden);
		}

		SetRenderOpacity(mCurrentFadeOpacity);
	}
}

void UAWeekStaminaWidget::NativeDestruct()
{
	StaminaChangedHandle.Unregister();

	Super::NativeDestruct();
}

void UAWeekStaminaWidget::UpdateProgress(float Stamina, bool bDecrease)
{
	Progress->SetPercent(Stamina / 100);

	if (bDecrease)
	{
		ShowWidget();

		mCurrentDelayTime = 0.0f;
		bStartFade = false;
		mCurrentFadeOpacity = 1.0f;

		SetRenderOpacity(mCurrentFadeOpacity);
	}
}
