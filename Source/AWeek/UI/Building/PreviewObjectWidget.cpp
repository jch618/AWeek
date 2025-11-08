// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Building/PreviewObjectWidget.h"

#include "AWeek/Grid/GridPlacedSubsystem.h"
#include "Input/CommonUIInputTypes.h"

void UPreviewObjectWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	//Deactive attack
	/*if (ULocalPlayer* LP = GetOwningLocalPlayer())
	{
		if (auto* Subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP))
		{
			if (AttackMappingContext)
			{
				Subsys->RemoveMappingContext(AttackMappingContext);
			}
		}
	}*/
	//SetInputConfig(FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture));

	if (GridPlacedSubsystem == nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			GridPlacedSubsystem = World->GetSubsystem<UGridPlacedSubsystem>();
		}
	}

	
	/*const FUIActionTag LTag = FUIActionTag::TryConvert(FGameplayTag::RequestGameplayTag(TEXT("UI.Action.LeftClick"), /*bErrorIfNotFound#1#false));
	const FUIActionTag RTag = FUIActionTag::TryConvert(FGameplayTag::RequestGameplayTag(TEXT("UI.Action.RightClick"), /*bErrorIfNotFound#1#false));*/

	/*if (!LTag.IsValid() || !RTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("PreviewWidget: UI Action Tag not valid (Left:%d Right:%d)"), LTag.IsValid(), RTag.IsValid());
		// 태그 매핑이 아직이면 조용히 리턴
		return;
	}*/
	

	UE_LOG(LogTemp, Log, TEXT("PreviewWidget click handler Binding!! a"))
	
	FBindUIActionArgs ArgsL(FUIActionTag::ConvertChecked(FGameplayTag::RequestGameplayTag(TEXT("UI.Action.LeftClick"))),
		false,
		FSimpleDelegate::CreateUObject(this, &UPreviewObjectWidget::SetupBuilding));
	LeftClickBindingHandle = RegisterUIActionBinding(ArgsL);

	FBindUIActionArgs ArgsR(FUIActionTag::ConvertChecked(FGameplayTag::RequestGameplayTag(TEXT("UI.Action.RightClick"))),
		false,
		FSimpleDelegate::CreateUObject(this, &UPreviewObjectWidget::Cancel));
	RightClickBindingHandle = RegisterUIActionBinding(ArgsR);

	FBindUIActionArgs ArgsWheelUp(FUIActionTag::ConvertChecked(FGameplayTag::RequestGameplayTag(TEXT("UI.Action.WheelUp"))),
		false,
		FSimpleDelegate::CreateUObject(this, &UPreviewObjectWidget::RotateR));
	WheelUpBindingHandle = RegisterUIActionBinding(ArgsWheelUp);

	FBindUIActionArgs ArgsWheelDown(FUIActionTag::ConvertChecked(FGameplayTag::RequestGameplayTag(TEXT("UI.Action.WheelDown"))),
		false,
		FSimpleDelegate::CreateUObject(this, &UPreviewObjectWidget::RotateL));
	WheelDownBindingHandle = RegisterUIActionBinding(ArgsWheelDown);
}

void UPreviewObjectWidget::NativeOnDeactivated()
{
	
	Super::NativeOnDeactivated();
}

void UPreviewObjectWidget::SetupBuilding()
{
	UE_LOG(LogTemp, Log, TEXT("Preview object Widget LeftClick active!!"));


	GridPlacedSubsystem->ConfirmPlacement();
	
}

void UPreviewObjectWidget::Cancel()
{
	UE_LOG(LogTemp, Log, TEXT("Preview object Widget RightClick active!!"));

	GridPlacedSubsystem->StopPlacement();
}

void UPreviewObjectWidget::RotateR()
{
	GridPlacedSubsystem->RotatePreview(90.0f);
}

void UPreviewObjectWidget::RotateL()
{
	GridPlacedSubsystem->RotatePreview(-90.0f);
}


