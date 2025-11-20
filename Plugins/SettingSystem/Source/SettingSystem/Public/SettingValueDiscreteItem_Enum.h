// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingValueDiscreteItem.h"
#include "SettingValueDiscreteItem_Enum.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingValueDiscreteItem_Enum : public USettingValueDiscreteItem
{
	GENERATED_BODY()
	
public:
	template<typename EnumType>
	void AddEnumOption(EnumType Key, const FText& ValueText)
	{
		const FString KeyString = StaticEnum<EnumType>()->GetNameStringByValue(Key);
		AddOption(KeyString, ValueText);
	}
	
	template<typename EnumType>
	EnumType GetValueEnum() const
	{
		const FString ValueString = GetValue();
		return StaticEnum<EnumType>()->GetValueByNameString(ValueString);
	}

	template<typename EnumType>
	void SetValueEnum(EnumType InValue)
	{
		const FString ValueString = StaticEnum<EnumType>()->GetNameStringByValue(InValue);
		SetValue(ValueString);
	}

	template<typename EnumType>
	void SetDefaultValueEnum(EnumType InValue)
	{
		const FString ValueString = StaticEnum<EnumType>()->GetNameStringByValue(InValue);
		DefaultValue = ValueString;
	}
};
