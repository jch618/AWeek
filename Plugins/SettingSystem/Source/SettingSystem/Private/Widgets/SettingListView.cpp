// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingListView.h"

#include "SettingItem.h"
#include "SettingItemCategory.h"
#include "Widgets/SettingListViewItem.h"
#include "Widgets/SettingWidgetTypeData.h"

TSubclassOf<UUserWidget> USettingListView::GetDesiredEntryClassForItem(UObject* Item) const
{
	if (const USettingItem* Setting = Cast<USettingItem>(Item))
	{
		if (TSubclassOf<UUserWidget> SettingClass = WidgetTypeData->GetListViewItemForSetting(Setting))
		{
			return SettingClass;
		}
	}
	return Super::GetDesiredEntryClassForItem(Item);
}

UUserWidget& USettingListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	USettingItem* Setting = Cast<USettingItem>(Item);
	UUserWidget& EntryWidget = Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	if (USettingListViewItem* Entry = Cast<USettingListViewItem>(&EntryWidget))
	{
		Entry->Init(Setting);
	}
	
	return EntryWidget;
}

bool USettingListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	if (USettingItemCategory* Setting = Cast<USettingItemCategory>(FirstSelectedItem))
	{
		return false;
	}

	return true;
}
