// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_AddWidget.h"

#include "AnimationSharingManager.h"
#include "AnimationSharingManager.h"
#include "CommonUIExtensions.h"
#include "AWeek/UI/AWeekHUD.h"
#include "Components/GameFrameworkComponentManager.h"

void UGameFeatureAction_AddWidget::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	AAWeekHUD* HUD = CastChecked<AAWeekHUD>(Actor);

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		for (const FAWeekHUDLayoutRequest& Entry: Layout)
		{
			if (TSubclassOf<UCommonActivatableWidget> ConcrepteWidgetClass = Entry.LayoutClass.Get())
			{
				ActiveData.LayoutAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcrepteWidgetClass));
			}
		}

		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FAWeekHUDElementEntry& Entry: Widgets)
		{
			ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(), -1));
		}
	}
}

void UGameFeatureAction_AddWidget::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout: ActiveData.LayoutAdded)
	{
		if (AddedLayout.IsValid())
		{
			AddedLayout->DeactivateWidget();
		}
	}

	ActiveData.LayoutAdded.Reset();

	for (FUIExtensionHandle& Handle: ActiveData.ExtensionHandles)
	{
		Handle.Unregister();
	}

	ActiveData.ExtensionHandles.Reset();
}

void UGameFeatureAction_AddWidget::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if ensure(ActiveData)
	{
		ActiveData->ComponentRequests.Empty();

		for (FUIExtensionHandle& Handle : ActiveData->ExtensionHandles)
		{
			Handle.Unregister();
		}
		ActiveData->LayoutAdded.Empty();
		ActiveData->ExtensionHandles.Empty();
	}
}

void UGameFeatureAction_AddWidget::AddToWorld(const FWorldContext& WorldContext,
	const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			TSoftClassPtr<AActor> HUDActorClass = AAWeekHUD::StaticClass();

			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HUDActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));
			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddWidget::HandleActorExtension(AActor* Actor, FName EventName,
	FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) || (EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
	{
		AddWidgets(Actor, ActiveData);
	}
}
