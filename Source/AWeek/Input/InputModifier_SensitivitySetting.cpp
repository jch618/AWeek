// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModifier_SensitivitySetting.h"

#include "AWeek/Settings/AWeekGameUserSettings.h"

FInputActionValue UInputModifier_SensitivitySetting::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                              FInputActionValue CurrentValue,
                                                                              float DeltaTime)
{
	if (UserSettings == nullptr)
	{
		UserSettings = UAWeekGameUserSettings::Get();
	}

	if (UserSettings)
	{
		const UClass* Class = UAWeekGameUserSettings::StaticClass();

		const FProperty* PropertyX = Properties.IsEmpty() ? Class->FindPropertyByName(AxisSettingNameX) : Properties[0];
		const FProperty* PropertyY = Properties.IsEmpty() ? Class->FindPropertyByName(AxisSettingNameY) : Properties[1];

		if (Properties.IsEmpty())
		{
			Properties.Emplace(PropertyX);
			Properties.Emplace(PropertyY);
		}

		FVector Scalar = FVector(1.0f, 1.0f, 1.0f);
		Scalar.X = PropertyX ? *PropertyX->ContainerPtrToValuePtr<float>(UserSettings) : 1.0f;
		Scalar.Y = PropertyY ? *PropertyY->ContainerPtrToValuePtr<float>(UserSettings) : 1.0f;
		return CurrentValue.Get<FVector>() * Scalar;
	}

	return CurrentValue;
}
