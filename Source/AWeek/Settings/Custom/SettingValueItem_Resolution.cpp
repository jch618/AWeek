// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingValueItem_Resolution.h"
#include "RHI.h"

#include "GameFramework/GameUserSettings.h"

#define LOCTEXT_NAMESPACE "AWeekSettings"

void USettingValueItem_Resolution::Init()
{
	Super::Init();

	Resolutions.Empty();
	
	// RHI가 제공하는 해상도 정보 구조체
	FScreenResolutionArray RHIResolutions;

	// 최소/최대 허용 해상도 조건
	const int32 MinWidth  = 640;
	const int32 MinHeight = 480;
	const int32 MaxWidth  = 2560;
	const int32 MaxHeight = 1440;

	// RHI에서 가능한 해상도 목록 요청
	if (RHIGetAvailableResolutions(RHIResolutions, true))
	{
		for (const FScreenResolutionRHI& RHIRes : RHIResolutions)
		{
			// 조건 필터 (너무 작은 해상도 제외)
			if (RHIRes.Width >= MinWidth &&
				RHIRes.Height >= MinHeight &&
				RHIRes.Width <= MaxWidth &&
				RHIRes.Height <= MaxHeight)
			{
				Resolutions.Emplace(RHIRes.Width, RHIRes.Height);

				const FString ResolutionKey = FString::Printf(TEXT("%dx%d"),
				(int32)RHIRes.Width, (int32)RHIRes.Height);
				
				AddOption(ResolutionKey, FText::FromString(ResolutionKey));
			}
		}
	}
}

void USettingValueItem_Resolution::Store()
{
}

void USettingValueItem_Resolution::Restore()
{
}

void USettingValueItem_Resolution::Reset()
{
}

int32 USettingValueItem_Resolution::GetValueOptionIndex() const
{
	if (const UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
	{
		FIntPoint ScreenResolution = UserSettings->GetScreenResolution();
		for (int32 i = 0;i<Resolutions.Num();i++)
		{
			if (Resolutions[i] == ScreenResolution)
			{
				return i;
			}
		}
	}
	
	return Resolutions.Num() - 1;;
}

void USettingValueItem_Resolution::SetValueOptionByIndex(int32 Index)
{
	if (Index > 0 && Index < Resolutions.Num())
	{
		if (UGameUserSettings* UserSettings = GEngine->GetGameUserSettings())
		{
			UserSettings->SetScreenResolution(Resolutions[Index]);
		}
	}
	
	NotifySettingChanged(ESettingChangedReason::Change);
}
