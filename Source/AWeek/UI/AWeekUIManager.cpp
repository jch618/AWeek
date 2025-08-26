// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekUIManager.h"

CAWeekUIManager* CAWeekUIManager::mInst = nullptr;

CAWeekUIManager::CAWeekUIManager()
{
}

CAWeekUIManager::~CAWeekUIManager()
{
}

void CAWeekUIManager::AddWidget(const FName& WidgetName, UAWeekUserWidget* Widget)
{
	// 같은 이름으로 등록된 위젯이 있을 경우
	if (mWidgetMap.Contains(WidgetName))
		return;

	mWidgetMap.Add(WidgetName, Widget);
}

void CAWeekUIManager::DeleteWidget(const FName& WidgetName)
{
	if (!mWidgetMap.Contains(WidgetName))
		return;

	mWidgetMap.Remove(WidgetName);
}

