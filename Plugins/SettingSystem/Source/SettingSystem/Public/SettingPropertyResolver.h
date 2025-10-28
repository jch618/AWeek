// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

/**
 * 
 */
// C++ Class이기 때문에 스마트 포인터로 관리 필요
class SETTINGSYSTEM_API FSettingPropertyResolver: public TSharedFromThis<FSettingPropertyResolver>
{
public:
	FSettingPropertyResolver(ULocalPlayer* InLocalPlayer, const TArray<FString>& InPropertyPath);
	~FSettingPropertyResolver();

	FString GetValue() const;
	void SetValue(const FString& Value);
private:
	FCachedPropertyPath PropertyPath;
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;
};
