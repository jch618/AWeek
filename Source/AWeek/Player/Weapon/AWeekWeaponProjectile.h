// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Actor.h"
#include "../../System/IDamageAble.h"
#include "AWeekWeaponProjectile.generated.h"

UCLASS()
class AWEEK_API AAWeekWeaponProjectile : public AActor, public IDamageAble
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAWeekWeaponProjectile();

protected:
	TObjectPtr<UBoxComponent> mBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpeed;

	TObjectPtr<UProjectileMovementComponent> mMovement;

	float Duration = 10.f;

	class AController* mOwnerController;

	float mDamage = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void SetDamage(float Damage)
	{
		mDamage = Damage;
	}

	void SetOwnerController(class AController* Controller)
	{
		mOwnerController = Controller;
	}

protected:
	UFUNCTION()
	void ProjectileStop(const FHitResult& Hit);
};
