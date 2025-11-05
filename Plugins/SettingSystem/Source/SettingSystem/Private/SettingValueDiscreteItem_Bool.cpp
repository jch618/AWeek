// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingValueDiscreteItem_Bool.h"

#define LOCTEXT_NAMESPACE "SettingValueDiscreteItemBool"

void USettingValueDiscreteItem_Bool::Init()
{
	Super::Init();
	AddOption(TEXT("true"), LOCTEXT("ON", "ON"));
	AddOption(TEXT("false"), LOCTEXT("OFF", "OFF"));
}

void USettingValueDiscreteItem_Bool::SetDefaultValue(bool InValue)
{
	DefaultValue = LexToString(InValue);
}
