// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingValueScalarItem.h"

#include "SettingPropertyResolver.h"

void USettingValueScalarItem::Reset()
{
	if (DefaultValue.IsSet())
	{
		SetValue(DefaultValue.GetValue());
	}
}

void USettingValueScalarItem::Store()
{
	InitialValue = GetValue();
}

void USettingValueScalarItem::Restore()
{
	SetValue(InitialValue);
}

void USettingValueScalarItem::SetInitialValue(double InValue)
{
	InitialValue = InValue;
}

void USettingValueScalarItem::SetValue(double InValue)
{
	if (MinValue.IsSet())
	{
		InValue = FMath::Max(InValue, MinValue.GetValue());
	}

	if (MaxValue.IsSet())
	{
		InValue = FMath::Min(InValue, MaxValue.GetValue());
	}

	const FString ValueString = LexToString(InValue);
	Setter->SetValue(ValueString);

	NotifySettingChanged();
}

double USettingValueScalarItem::GetValue()
{
	double Value;
	const FString ValueString = Getter->GetValue();
	
	LexFromString(Value, *ValueString);
	
	return Value;
}

void USettingValueScalarItem::SetMinValue(double InMinValue)
{
	MinValue = InMinValue;
}

void USettingValueScalarItem::SetMaxValue(double InMaxValue)
{
	MaxValue = InMaxValue;
}

double USettingValueScalarItem::GetMinValue()
{
	return MinValue.GetValue();
}

double USettingValueScalarItem::GetMaxValue()
{
	return MaxValue.GetValue();
}

void USettingValueScalarItem::SetGetter(const TSharedRef<FSettingPropertyResolver>& InGetter)
{
	Getter = InGetter;
}

void USettingValueScalarItem::SetSetter(const TSharedRef<FSettingPropertyResolver>& InSetter)
{
	Setter = InSetter;
}
