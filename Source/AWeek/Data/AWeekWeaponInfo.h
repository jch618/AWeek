// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/Weapon/AWeekReticleDefinition.h"
#include "AWeekWeaponInfo.generated.h"

UENUM()
enum class EWeaponType
{
	Unarmed,
	Melee,
	Ranged
};

USTRUCT(BlueprintType)
struct FRangedWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRuntimeFloatCurve HeatShotCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRuntimeFloatCurve HeatShotRecoveryCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FRuntimeFloatCurve HeatToSpreadCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="0.0"))
	float CenterBias;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="0.0"))
	float HeatShotRecoveryDelay;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TransitSpeed = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="1.0", ClampMax="4.0", ForceUnits=x))
	float JumpSpreadMultiplierNormal = 1.0f; // Jump시 곱해질 Spread Multiplier

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="1.0", ClampMax="2.0", ForceUnits=x))
	float MoveSpreadMultiplierNormal = 1.0f; // Move시 곱해질 Spread Multiplier (1.0 ~ 서서히 증가)
	
	float CurrentHeat = 0.0f;
	float CurrentSpreadAngle = 0.0f; // 현재 Spread Angle 
	
	FRangedWeaponInfo()
	{
		HeatShotCurve.EditorCurveData.AddKey(0.0f, 1.0f);
		HeatShotRecoveryCurve.EditorCurveData.AddKey(0.0f, 2.0f);
	}
	
	static FVector GetRandomDirectionInCone(const FVector& Direction, float SpreadAngleDegree, float Bias = 0.0f, float SigmaScale = 1.0f);

	float ClampHeatRange(float HeatValue);
	void ComputeHeatRange(const FRuntimeFloatCurve& RuntimeFloatCurve, float& MinTime, float& MaxTime);
	
	float CurrentSpreadMultiplier = 1.0f;
	float StandingSpreadMultiplier = 0.0f;
	float JumpSpreadMultiplier = 0.0f;

	
};

USTRUCT(BlueprintType)
struct FAWeekWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> Projectile;

	UPROPERTY(EditAnywhere)
	int32 BulletMaxStack;

	UPROPERTY(EditAnywhere)
	int32 BulletUsagePerSingle;

	UPROPERTY(EditAnywhere)
	float FireRate;

	UPROPERTY(EditAnywhere)
	UParticleSystem* FireEffect;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAWeekReticleDefinition> ReticleDefinition;

	UPROPERTY(EditAnywhere)
	FRangedWeaponInfo RangedWeaponInfo;
};
