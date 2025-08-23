// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonUIExtensions.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "PrimaryGameLayout.generated.h"

class UCommonActivatableWidget;

enum class EAsyncWidgetLayerState : uint8
{
	Canceled,
	Initialize,
	AfterPush
};

/**
 * 
 */
UCLASS(Abstract)
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	static UPrimaryGameLayout* GetPrimaryGameLayout(APlayerController* PlayerController);
	static UPrimaryGameLayout* GetPrimaryGameLayout(ULocalPlayer* LocalPlayer);

	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerName);

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass)
	{
		return PushWidgetToLayerStack<ActivatableWidgetT>(LayerName, ActivatableWidgetClass, [](ActivatableWidgetT&)
		{
		});
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	ActivatableWidgetT* PushWidgetToLayerStack(FGameplayTag LayerName, UClass* ActivatableWidgetClass,
	                                           TFunctionRef<void(ActivatableWidgetT&)> InitInstanceFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived,
					  "only CommonActivatableWidgets!!!!!!");

		if (UCommonActivatableWidgetContainerBase* Layer = GetLayerWidget(LayerName))
		{
			return Layer->AddWidget<ActivatableWidgetT>(ActivatableWidgetClass, InitInstanceFunc);
		}

		return nullptr;
	}

	template <typename ActivatableWidgetT = UCommonActivatableWidget>
	TSharedPtr<FStreamableHandle> PushWidgetToLayerStackAsync(FGameplayTag LayerName, bool bSuspendInputUntilComplete,
	                                                          TSoftClassPtr<UCommonActivatableWidget>
	                                                          ActivatableWidgetClass,
	                                                          TFunction<void(
		                                                          EAsyncWidgetLayerState,
		                                                          ActivatableWidgetT*)> StateFunc)
	{
		static_assert(TIsDerivedFrom<ActivatableWidgetT, UCommonActivatableWidget>::IsDerived,
		              "Only CommonActivatableWidgets");

		static FName NAME_PushingWidgetToLayer("PushingWidgetToLayer");
		const FName SuspendInputToken = bSuspendInputUntilComplete
			                                ? UCommonUIExtensions::SuspendInputForPlayer(
				                                GetOwningPlayer(), NAME_PushingWidgetToLayer)
			                                : NAME_None;

		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		TSharedPtr<FStreamableHandle> StreamingHandle = StreamableManager.RequestAsyncLoad(
			ActivatableWidgetClass.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this,
				[this, LayerName, ActivatableWidgetClass, StateFunc, SuspendInputToken]()
				{
					UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);

					ActivatableWidgetT* Widget = PushWidgetToLayerStack<ActivatableWidgetT>(
						LayerName, ActivatableWidgetClass.Get(), [StateFunc](ActivatableWidgetT& WidgetToInit)
						{
							StateFunc(EAsyncWidgetLayerState::Initialize, &WidgetToInit);
						});

					StateFunc(EAsyncWidgetLayerState::AfterPush, Widget);
				})
		);

		StreamingHandle->BindCancelDelegate(FStreamableDelegate::CreateWeakLambda(this,
				[this, StateFunc, SuspendInputToken]()
				{
					UCommonUIExtensions::ResumeInputForPlayer(GetOwningPlayer(), SuspendInputToken);
					StateFunc(EAsyncWidgetLayerState::Canceled, nullptr);
				})
		);

		return StreamingHandle;
	}

	UFUNCTION(BlueprintCallable, Category = "Layer")
	void RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);
	
	UPROPERTY(Transient, meta = (Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
