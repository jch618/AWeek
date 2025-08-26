// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekUserWidget.h"
#include "AWeekUIManager.h"

UAWeekUserWidget::UAWeekUserWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UAWeekUserWidget::PlayWidgetAnimation(const FString& Name, float PlayStartTime, float PlaySpeed, bool Forward, bool RestoreState, int32 LoopCount)
{
	TObjectPtr<UWidgetAnimation>	Anim = mAnimMap.FindRef(Name);

	if (IsValid(Anim))
	{
		EUMGSequencePlayMode::Type	PlayMode = EUMGSequencePlayMode::Forward;

		if (!Forward)
			PlayMode = EUMGSequencePlayMode::Reverse;

		PlayAnimation(Anim, PlayStartTime, LoopCount, PlayMode, PlaySpeed, RestoreState);
	}
}

TObjectPtr<UWidgetAnimation> UAWeekUserWidget::GetWidgetAnimation(const FString& Name)
{
	TObjectPtr<UWidgetAnimation> Anim = mAnimMap.FindRef(Name);
	if (IsValid(Anim))
		return Anim;
	return nullptr;
}

void UAWeekUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CAWeekUIManager::GetInst()->AddWidget(mWidgetName, this);
}

void UAWeekUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UAWeekUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	for (auto& Anim : WidgetClass->Animations)
	{
		// formatting: strip _INST 
		FString	Name = Anim->GetName();

		Name.ReplaceInline(TEXT("_INST"), TEXT(""), ESearchCase::IgnoreCase);

		mAnimMap.Add(Name, Anim);
	}

	bIsFocusable = true;
}

void UAWeekUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	CAWeekUIManager::GetInst()->DeleteWidget(mWidgetName);
}
