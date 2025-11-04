// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingValueItem.h"
#include "SettingValueScalarItem.generated.h"

class FSettingPropertyResolver;
/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingValueScalarItem : public USettingValueItem
{
	GENERATED_BODY()
public:
	virtual void Reset() override;
	virtual void Store() override;
	virtual void Restore() override;

	void SetInitialValue(double InValue);
	
	void SetValue(double InValue);
	double GetValue();

	void SetMinValue(double InMinValue);
	void SetMaxValue(double InMaxValue);
	
	double GetMinValue();
	double GetMaxValue();

	void SetGetter(const TSharedRef<FSettingPropertyResolver>& InGetter);
	void SetSetter(const TSharedRef<FSettingPropertyResolver>& InSetter);

private:
	TOptional<double> DefaultValue;
	double InitialValue;
	
	TOptional<double> MinValue;
	TOptional<double> MaxValue;

	TSharedPtr<FSettingPropertyResolver> Getter;
	TSharedPtr<FSettingPropertyResolver> Setter;
};
