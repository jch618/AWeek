// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Components/TreeView.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"

#include "Animation/WidgetAnimation.h"

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

#include "AWeekUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAWeekUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName		mWidgetName;

	UPROPERTY(BlueprintReadOnly)
	TMap<FString, TObjectPtr<UWidgetAnimation>>	mAnimMap;

	AActor* mOwnerActor = nullptr;
	TObjectPtr<UUserWidget>	mParentWidget;

	int32		mFocusZOrder = -1;

public:
	void SetOwnerActor(AActor* OwnerActor)
	{
		mOwnerActor = OwnerActor;
	}

	void SetParentWidget(UUserWidget* ParentWidget)
	{
		mParentWidget = ParentWidget;
	}

	void PlayWidgetAnimation(const FString& Name, float PlayStartTime = 0.f, float PlaySpeed = 1.f, bool Forward = true, bool RestoreState = false, int32 LoopCount = 1);
	TObjectPtr<UWidgetAnimation> GetWidgetAnimation(const FString& Name);

protected:
	virtual void NativeOnInitialized();
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();	
};
