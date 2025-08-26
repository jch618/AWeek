// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionWarpingComponent.h"
#include "Components/ActorComponent.h"
#include "AWeekPakourComponent.generated.h"

UENUM()
enum class ETraceType
{
	Line,
	Sphere
};

UCLASS( ClassGroup=(Pakour), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UAWeekPakourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAWeekPakourComponent();

protected:
	UPROPERTY(VisibleAnywhere)
	float mDetectedWallHeight = -1;

	TObjectPtr<class AAWeekCharacter> mOwner;
	TObjectPtr<UMotionWarpingComponent> mOwnerMWC;

	// 벽 맨 위쪽
	FHitResult mFirstWallHit;

	// 벽 맨 위에서 맨 앞쪽
	FHitResult mFirstTopHit;

	// 벽 맨 위에서 맷 뒷쪽
	FHitResult mLastTopHit;

	// 벽 맨 뒷쪽의 ImpactPoint
	FHitResult mEndOfWallHit;

	// 벽 맨 뒷쪽의 ImpactPoint에서 플레이어 키만큼 떨군지점 즉, 볼트 착지지점
	FHitResult mVaultLandHit;

	// 벽의 역노말 벡터
	FVector mWallRotation = FVector::ZeroVector;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere)
	bool bCanPakour = true;

	void TriggerPakour();
	FHitResult DetectWall();
	void ScanWall(FHitResult Hit);
	void SetMotionWarping();
	void TryVault();

protected:
	// Utility Functions
	FHitResult WallTracing(ETraceType TraceType, FVector Start, FVector End, FColor Color=FColor::Green, bool Debug = true,
	ECollisionChannel CollisionChannel = ECollisionChannel::ECC_Visibility, float Radius = 10)
	{
		FHitResult HitResult;
		bool bHit;

		if (TraceType==ETraceType::Line)
		{
			bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				Start,
				End,
				ECollisionChannel::ECC_Visibility,
				FCollisionQueryParams(),
				FCollisionResponseParams()
			);
		}
		else
		{
			FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
			bHit = GetWorld()->SweepSingleByChannel(
				HitResult,
				Start,
				End,
				FQuat::Identity,
				ECollisionChannel::ECC_Visibility,
				Sphere,
				FCollisionQueryParams(),
				FCollisionResponseParams()
			);
		}

		if (Debug)
		{
			if (TraceType == ETraceType::Line)
			{
				
				DrawDebugLine(
					GetWorld(),
					Start,
					End,
					Color,
					false,
					0.1f,
					0,
					2.0f
				);
			}

			else
			{
				FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
				DrawDebugCapsule(
					GetWorld(),
					bHit ? HitResult.Location : End,
					Sphere.GetCapsuleHalfHeight(),
					Sphere.GetCapsuleRadius(),
					FQuat::Identity,
					Color,
					false,
					0.1f
				);
			}
		}

		return HitResult;
	}
	
};
