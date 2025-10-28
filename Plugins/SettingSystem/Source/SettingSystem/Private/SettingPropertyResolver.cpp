// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingPropertyResolver.h"

FSettingPropertyResolver::FSettingPropertyResolver(ULocalPlayer* InLocalPlayer, const TArray<FString>& InPropertyPath)
	:LocalPlayer(InLocalPlayer),PropertyPath(InPropertyPath)
{
}

FSettingPropertyResolver::~FSettingPropertyResolver()
{
}

FString FSettingPropertyResolver::GetValue() const
{
	FString OutValue;
	PropertyPathHelpers::GetPropertyValueAsString(LocalPlayer.Get(), PropertyPath, OutValue);
	return OutValue;
}

void FSettingPropertyResolver::SetValue(const FString& Value)
{
	PropertyPathHelpers::SetPropertyValueFromString(LocalPlayer.Get(), PropertyPath, Value);
}
