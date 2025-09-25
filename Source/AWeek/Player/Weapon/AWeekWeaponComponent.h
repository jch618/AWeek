// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekWeapon.h"
#include "../../Data/AWeekWeaponInfo.h"
#include "../../Character/AWeekPlayerCharacter.h"
#include "AWeekWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UAWeekWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAWeekWeaponComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AAWeekPlayerCharacter> mOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AAWeekWeapon> mWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EWeaponType mWeaponType = EWeaponType::Unarmed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float mDamage = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor>	mProjectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32	mBulletMaxStack = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32	mBulletUsagePerSingle = 0;

	TObjectPtr<UStaticMeshComponent> mWeaponMeshComp;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

public:	
	float GetWeaponDamage()
	{
		return mDamage;
	}

	FVector GetWeaponMuzzle()
	{
		return mWeapon->mMeshComponent->GetSocketLocation(FName("Muzzle"));
	}

public:
	void ChangeWeapon(FName WeaponKey);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAWeekReticleDefinition> mReticleDefinition;
};
