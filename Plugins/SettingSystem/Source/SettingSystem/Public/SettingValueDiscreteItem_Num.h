// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingValueDiscreteItem.h"
#include "SettingValueDiscreteItem_Num.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingValueDiscreteItem_Num : public USettingValueDiscreteItem
{
	GENERATED_BODY()
public:
	template<typename NumType>
	void AddNumOption(NumType Key, const FText& ValueText)
	{
		const FString KeyString = LexToString(Key);
		AddOption(KeyString, ValueText);
	}
	
	template<typename NumType>
	NumType GetValueNum() const
	{
		const FString ValueString = GetValue();
		NumType Value;
		LexFromString(ValueString, *Value);
		
		return Value;
	}

	template<typename NumType>
	void SetValueNum(NumType InValue)
	{
		const FString ValueString = LexToString(InValue);
		SetValue(ValueString);
	}

	template<typename NumType>
	void SetDefaultValueNum(NumType InValue)
	{
		const FString ValueString = LexToString(InValue);
		DefaultValue = ValueString;
	}
};
