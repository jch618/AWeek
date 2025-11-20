// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingWidgetTypeData.h"

#include "SettingItem.h"
#include "Widgets/SettingListViewItem.h"

TSubclassOf<UUserWidget> USettingWidgetTypeData::GetListViewItemForSetting(const USettingItem* Setting) const
{
	UClass* Class = Setting->GetClass();
	while (Class)
	{
		if (TSubclassOf<USettingListViewItem> ListViewItem = EntryWidgetForClass.FindRef(Class))
		{
			return ListViewItem;
		}
		Class = Class->GetSuperClass();
	}
	
	return nullptr;
}
