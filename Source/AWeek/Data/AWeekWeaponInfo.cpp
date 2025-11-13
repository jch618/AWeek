// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeaponInfo.h"

FVector FRangedWeaponInfo::GetRandomDirectionInCone(const FVector& Direction, float SpreadAngleDegree, float Bias,
	float SigmaScale)
{
	const float U1 = FMath::Max(FMath::FRand(), KINDA_SMALL_NUMBER);
	const float U2 = FMath::FRand();
	const float Z = FMath::Sqrt(-2.0f * FMath::Loge(U1)) * FMath::Cos(2.0f * PI * U2);

	const float MaxRad = FMath::DegreesToRadians(SpreadAngleDegree * 0.5f);
	const float Sigma = (MaxRad / 3.0f) * FMath::Max(0.001f, SigmaScale);

	float Theta = FMath::Abs(Z * Sigma);
	Theta = FMath::Min(Theta, MaxRad);

	if (!FMath::IsNearlyZero(Bias))
	{
		float t = Theta / FMath::Max(MaxRad, KINDA_SMALL_NUMBER);
		t = FMath::Pow(t, Bias);
		Theta = t * MaxRad;
	}

	const float Phi = FMath::FRand() * 2.0f * PI;
	FVector N = Direction.GetSafeNormal();
	FVector TempUp = (FMath::Abs(N.Z) < 0.999f) ? FVector::UpVector : FVector::RightVector;
	FVector Right = FVector::CrossProduct(TempUp, N);
	FVector Up = FVector::CrossProduct(N, Right).GetSafeNormal();

	FVector Result = N * FMath::Cos(Theta) + (Right * FMath::Cos(Phi) + Up * FMath::Sin(Phi)) * FMath::Sin(Theta);
	
	return Result.GetSafeNormal();
}

float FRangedWeaponInfo::ClampHeatRange(float HeatValue)
{
	float MinTime, MaxTime;
	
	ComputeHeatRange(HeatShotCurve, MinTime, MaxTime);
	float MinHeat = MinTime;
	float MaxHeat = MaxTime;
	
	ComputeHeatRange(HeatShotRecoveryCurve, MinTime, MaxTime);
	MinHeat = FMath::Min(MinHeat, MinTime);
	MaxHeat = FMath::Max(MaxHeat, MinTime);

	ComputeHeatRange(HeatToSpreadCurve, MinTime, MaxTime);
	MinHeat = FMath::Min(MinHeat, MinTime);
	MaxHeat = FMath::Max(MaxHeat, MaxTime);

	//UE_LOG(LogTemp, Warning, TEXT("Min: %f, Max: %f"), MinHeat, MaxHeat);
	return FMath::Clamp(HeatValue, MinHeat, MaxHeat);
}

void FRangedWeaponInfo::ComputeHeatRange(const FRuntimeFloatCurve& RuntimeFloatCurve, float& MinTime, float& MaxTime)
{
	RuntimeFloatCurve.GetRichCurveConst()->GetTimeRange(MinTime, MaxTime);
}
