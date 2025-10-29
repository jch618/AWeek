// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/MainWidget/BuidlingButton.h"

#include "AWeek/Grid/GridPlacedSubsystem.h"
#include "AWeek/UI/MainWidget//MainUIWidget.h"
#include "CommonUIExtensions.h"
#include "Kismet/GameplayStatics.h"

void UBuidlingButton::NativeConstruct()
{
	Super::NativeConstruct();
	OnClicked().RemoveAll(this);
	OnClicked().AddUObject(this, &ThisClass::HandleClicked);
}

void UBuidlingButton::HandleClicked()
{
	if (ULocalPlayer* LP = GetOwningLocalPlayer())
	{
		/*if (bPopAll)
			UCommonUIExtensions::Pop*/
		if (UMainUIWidget* Main = GetTypedOuter<UMainUIWidget>())
		{
			Main->DeactivateWidget();      // 스택에서 내려감
			// Main->RemoveFromParent();   // 필요하면 제거까지

			UE_LOG(LogTemp, Log, TEXT("Close main ui widget"));
		}
		

		//TODO Create Preview Object
		UE_LOG(LogTemp, Log, TEXT("Preview Object1"));
		if (UWorld* W = GetWorld())
		{
			UE_LOG(LogTemp, Log, TEXT("Preview Object2"));
			if (UGridPlacedSubsystem* Sub = W->GetSubsystem<UGridPlacedSubsystem>())
			{
				UE_LOG(LogTemp, Log, TEXT("Preview Object3"));
				APlayerController* PC = UGameplayStatics::GetPlayerController(W, 0);

				if (!PreviewObjectClass)
				{
					UE_LOG(LogTemp, Log, TEXT("PreviewObjectClass is null"));
				}
				
				Sub->StartPlacement(PreviewObjectClass, PC);
			}
		}
	}
}

