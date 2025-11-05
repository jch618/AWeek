// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingValueDiscreteItem.h"

#include "SettingPropertyResolver.h"

void USettingValueDiscreteItem::Init()
{
	Super::Init();
}

void USettingValueDiscreteItem::Reset()
{
	if (DefaultValue.IsSet())
	{
		SetValue(DefaultValue.GetValue());
	}
}

void USettingValueDiscreteItem::Store()
{
	InitialValue = GetValue();
}

void USettingValueDiscreteItem::Restore()
{
	SetValue(InitialValue);
}

void USettingValueDiscreteItem::SetValue(const FString& InValue)
{
	Setter->SetValue(InValue);
}

FString USettingValueDiscreteItem::GetValue() const
{
	return Getter->GetValue();
}

void USettingValueDiscreteItem::SetGetter(const TSharedRef<FSettingPropertyResolver>& InGetter)
{
	Getter = InGetter;
}

void USettingValueDiscreteItem::SetSetter(const TSharedRef<FSettingPropertyResolver>& InSetter)
{
	Setter = InSetter;	
}

void USettingValueDiscreteItem::AddOption(const FString& Key, const FText& Value)
{
	OptionKey.Add(Key);
	OptionValue.Add(Value);
}

int32 USettingValueDiscreteItem::GetValueOptionIndex() const
{
	const FString CurrentKey = Getter->GetValue();
	for (int32 i=0;i<OptionKey.Num();i++)
	{
		if (OptionKey[i] == CurrentKey)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

void USettingValueDiscreteItem::SetValueOptionByIndex(int32 Index)
{
	if (Index >= 0 && Index < OptionKey.Num())
	{
		SetValue(OptionKey[Index]);
	}
}

TArray<FText>& USettingValueDiscreteItem::GetValueOptions()
{
	return OptionValue;
}
