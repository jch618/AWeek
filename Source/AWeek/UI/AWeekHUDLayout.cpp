// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekHUDLayout.h"

#include "CommonUIExtensions.h"
#include "UITag.h"
#include "AWeek/AWeekGameplayTags.h"
#include "Input/CommonUIInputTypes.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_ESCAPE, "UI.Action.Escape");

void UAWeekHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	FUIActionTag ActionTag = FUIActionTag::TryConvert(TAG_UI_ACTION_ESCAPE);
	if (ActionTag.IsValid())
	{
		RegisterUIActionBinding(FBindUIActionArgs(ActionTag, false, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleEscapeAction)));
	}
}

void UAWeekHUDLayout::NativeDestruct()
{
	Super::NativeDestruct();
}

void UAWeekHUDLayout::HandleEscapeAction()
{
	if (ensure(!EscapeMenuClass.IsNull()))
	{
		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU, EscapeMenuClass);
	}
}

