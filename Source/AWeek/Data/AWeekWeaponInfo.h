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
struct FAWeekWeaponInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh>	Mesh;

	UPROPERTY(EditAnywhere)
	EWeaponType	WeaponType;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> Projectile;

	UPROPERTY(EditAnywhere)
	int32 BulletMaxStack;

	UPROPERTY(EditAnywhere)
	int32 BulletUsagePerSingle;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UAWeekReticleDefinition> ReticleDefinition;
};