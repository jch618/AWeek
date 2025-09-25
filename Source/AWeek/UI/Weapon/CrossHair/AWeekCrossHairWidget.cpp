// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekCrossHairWidget.h"

#include "AWeek/Data/AWeekWeaponInfo.h"

static float GDebugFinalSpreadAngle = 45.0f;

static FAutoConsoleVariableRef CVarDebugFinalSpreadAngle(
	TEXT("aweek.DebugFinalSpreadAngle"),
	GDebugFinalSpreadAngle,
	TEXT("Adjust debug final spread angle for crosshair."),
	ECVF_Cheat
);

float UAWeekCrossHairWidget::GetFinalSpreadAngle() const
{
	EWeaponType WeaponType = EWeaponType::Ranged;
	if (WeaponType == EWeaponType::Ranged)
	{
		
	}

	return GDebugFinalSpreadAngle;
}

float UAWeekCrossHairWidget::GetScreenspaceMaxSpreadAngleRadius() const
{
	const float LongShotDistance = 10000.f;

	APlayerController* PC = GetOwningPlayer();
	if (PC && PC->PlayerCameraManager)
	{
		const float SpreadRadiusRads = FMath::DegreesToRadians(GetFinalSpreadAngle() * 0.5f);
		const float SpreadRadiusAtDistance = FMath::Tan(SpreadRadiusRads) * LongShotDistance;

		FVector CamPos;
		FRotator CamOrient;
		PC->PlayerCameraManager->GetCameraViewPoint(CamPos, CamOrient);

		FVector CamForwDir = CamOrient.RotateVector(FVector::ForwardVector);
		FVector CamUpDir   = CamOrient.RotateVector(FVector::UpVector);

		FVector OffsetTargetAtDistance = CamPos + (CamForwDir * LongShotDistance) + (CamUpDir * SpreadRadiusAtDistance);

		FVector2D OffsetTargetInScreenspace;

		if (PC->ProjectWorldLocationToScreen(OffsetTargetAtDistance, OffsetTargetInScreenspace, true))
		{
			int32 ViewportSizeX(0), ViewportSizeY(0);
			PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

			const FVector2D ScreenSpaceCenter(FVector::FReal(ViewportSizeX) * 0.5f, FVector::FReal(ViewportSizeY) * 0.5f);

			return (OffsetTargetInScreenspace - ScreenSpaceCenter).Length();
		}
	}
	
	return 0.0f;
}

bool UAWeekCrossHairWidget::HasFirstShotAccuracy() const
{
	EWeaponType WeaponType = EWeaponType::Ranged;
	if (WeaponType == EWeaponType::Ranged)
	{
		return true;
	}

	return false;
}

void UAWeekCrossHairWidget::SetFinalSpreadAngle(float NewFinalSpreadAngle)
{
	GDebugFinalSpreadAngle = NewFinalSpreadAngle;
}
