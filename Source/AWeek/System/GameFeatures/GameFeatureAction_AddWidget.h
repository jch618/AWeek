// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "GameFeatureAction_AddWidget.generated.h"

struct FComponentRequestHandle;

USTRUCT()
struct FAWeekHUDLayoutRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag LayerID;
};

USTRUCT()
struct FAWeekHUDElementEntry
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = UI)
	FGameplayTag SlotID;
};

UCLASS()
class AWEEK_API UGameFeatureAction_AddWidget : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutAdded;

		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	void AddWidgets(AActor* Actor, FPerContextData& ActiveData);
	void RemoveWidgets(AActor* Actor, FPerContextData& ActiveData);
	
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

	void HandleActorExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);
	
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;
	
	UPROPERTY(EditAnywhere, Category=UI)
	TArray<FAWeekHUDLayoutRequest> Layout;

	UPROPERTY(EditAnywhere, Category=UI)
	TArray<FAWeekHUDElementEntry> Widgets;
};
