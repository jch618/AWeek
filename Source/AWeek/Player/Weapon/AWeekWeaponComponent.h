// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekWeapon.h"
#include "../../Data/AWeekWeaponInfo.h"
#include "../../Character/AWeekPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AWeekWeaponComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AWEEK_API UAWeekWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAWeekWeaponComponent();

protected:
	TObjectPtr<UStaticMeshComponent> mWeaponMeshComp;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AAWeekPlayerCharacter> mOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AAWeekWeapon> mWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName mWeaponKey;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponType mWeaponType = EWeaponType::Unarmed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float mDamage = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> mProjectile;

	bool bOutOfBullet = false;

	int32 mCurrentBullet = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 mBulletMaxStack = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 mBulletUsagePerSingle = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float mFireRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystem* mFireEffect;

	/*--------------RANGED WEAPON--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsFiring;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsReloading;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mTimeSinceLastShot;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FVector GetFireDirection();

public:
	float GetWeaponDamage() { return mDamage; }
	FVector GetMuzzleLocation() { return mWeapon->mMeshComponent->GetSocketLocation(FName("Muzzle")); }
	FName GetWeaponKey() { return mWeaponKey; }
	EWeaponType GetWeaponType() { return mWeaponType; }
	float GetFireRate() { return mFireRate; }

public:
	void Fire();

	bool StartFire()
	{
		// �Ѿ� �پ��� false ����
		if (bOutOfBullet)
			return false;
		bIsFiring = true;
		return true;
	}

	void EndFire()
	{
		bIsFiring = false;
	}

	void ChangeWeapon(FName WeaponKey);
	void ChangeWeaponPos(FName SocketName);

	void Reload()
	{
		mCurrentBullet = mBulletMaxStack;
		bOutOfBullet = false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAWeekReticleDefinition> mReticleDefinition;

private:
	FRangedWeaponInfo RangedWeaponInfo;
public:
	float GetSpreadAngle() const
	{
		return RangedWeaponInfo.CurrentSpreadAngle;
	}

	float GetSpreadMultiplier() const
	{
		return RangedWeaponInfo.CurrentSpreadMultiplier;
	}

	float CalculateFinalSpreadAngle() const
	{
		return RangedWeaponInfo.CurrentSpreadAngle * RangedWeaponInfo.CurrentSpreadMultiplier;
	}

	void TickSpread(float DeltaTime);
	void TickMultipliers(float DeltaTime);
	void AddSpreadHeat();

};
