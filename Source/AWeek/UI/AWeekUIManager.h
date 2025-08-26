// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class AWEEK_API CAWeekUIManager
{
public:
	CAWeekUIManager();
	~CAWeekUIManager();

private:
	TMap<FName, TObjectPtr<class UAWeekUserWidget>>	mWidgetMap;

public:
	void AddWidget(const FName& WidgetName, class UAWeekUserWidget* Widget);
	void DeleteWidget(const FName& WidgetName);

	template <typename T>
	T* FindWidget(const FName& WidgetName)
	{
		if (!mWidgetMap.Contains(WidgetName))
			return nullptr;

		return Cast<T>(mWidgetMap.FindRef(WidgetName));
	}

private:
	static CAWeekUIManager* mInst;

public:
	static CAWeekUIManager* GetInst()
	{
		if (!mInst)
			mInst = new CAWeekUIManager;
		return mInst;
	}

	static void DestroyInst()
	{
		if (mInst)
		{
			delete mInst;
			mInst = nullptr;
		}
	}
};
