// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingListView.h"

TSubclassOf<UUserWidget> USettingListView::GetDesiredEntryClassForItem(UObject* Item) const
{
	return Super::GetDesiredEntryClassForItem(Item);
}
