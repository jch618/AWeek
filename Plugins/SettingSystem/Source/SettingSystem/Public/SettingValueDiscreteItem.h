// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingValueItem.h"
#include "SettingValueDiscreteItem.generated.h"

class FSettingPropertyResolver;
/**
 * 
 */

// FString 형식으로 관리 FString -> FText Value로 Display Text 전환
UCLASS()
class SETTINGSYSTEM_API USettingValueDiscreteItem : public USettingValueItem
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	virtual void Reset() override;
	virtual void Store() override;
	virtual void Restore() override;

	void SetValue(const FString& InValue);
	FString GetValue() const;

	void SetGetter(const TSharedRef<FSettingPropertyResolver>& InGetter);
	void SetSetter(const TSharedRef<FSettingPropertyResolver>& InSetter);

	void AddOption(const FString& Key, const FText& Value);
	
	virtual int32 GetValueOptionIndex() const;
	virtual void SetValueOptionByIndex(int32 Index);

	TArray<FText>& GetValueOptions();
protected:
	TOptional<FString> DefaultValue;
	FString InitialValue;

	TSharedPtr<FSettingPropertyResolver> Getter;
	TSharedPtr<FSettingPropertyResolver> Setter;

	TArray<FString> OptionKey;
	TArray<FText> OptionValue;
};
