// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeaponComponent.h"

#include <AWeek/Components/AWeekPlayerInventoryComponent.h>

#include "../../Components/AWeekInventoryComponent.h"
#include "../../AWeekAssetManager.h"
#include "AWeekWeaponProjectile.h"

// Sets default values for this component's properties
UAWeekWeaponComponent::UAWeekWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAWeekWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAWeekWeaponComponent::OnRegister()
{
	Super::OnRegister();
	mOwner = Cast<AAWeekPlayerCharacter>(GetOwner());
	CharacterMovementComponent = Cast<UCharacterMovementComponent>(mOwner->GetMovementComponent());
	mWeapon = GetWorld()->SpawnActor<AAWeekWeapon>();
	mWeaponMeshComp = mWeapon->mMeshComponent;
	if (mWeapon)
	{
		mWeapon->AttachToComponent(
			mOwner->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("Default")
		);
	}
}

void UAWeekWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (mWeaponType == EWeaponType::Ranged)
	{
		mTimeSinceLastShot += DeltaTime;
		TickSpread(DeltaTime);
		TickMultipliers(DeltaTime);
	}
	if (bIsFiring && mCurrentBullet > 0 && mWeaponType == EWeaponType::Ranged)
	{
		// �߻� ���� ��� (RPS)
		float FireInterval = 1.f / mFireRate;
		if (mTimeSinceLastShot >= FireInterval)
		{
			Fire();
			mTimeSinceLastShot = 0.0f;
		}
	}
}

void UAWeekWeaponComponent::ChangeWeapon(FName WeaponKey)
{
	UDataTable* WeaponInfoDT = UAWeekAssetManager::Get().FindDataTable(TEXT("DT_WeaponInfo"));
	if (!WeaponInfoDT)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon DataTable Not Found"));
		return;
	}

	FAWeekWeaponInfo* WeaponInfo = WeaponInfoDT->FindRow<FAWeekWeaponInfo>(WeaponKey, TEXT("Lookup"));
	if (!WeaponInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon DataTable Row Not Found : %s"), *WeaponKey.ToString());
		return;
	}

	UStaticMesh* Mesh = WeaponInfo->Mesh;
	mWeaponKey = WeaponKey;
	mWeaponType = WeaponInfo->WeaponType;
	mDamage = WeaponInfo->Damage;
	mProjectile = WeaponInfo->Projectile;
	mCurrentBullet = WeaponInfo->BulletMaxStack;
	mBulletMaxStack = WeaponInfo->BulletMaxStack;
	mBulletUsagePerSingle = WeaponInfo->BulletUsagePerSingle;
	mFireRate = WeaponInfo->FireRate;
	mFireEffect = WeaponInfo->FireEffect;
	mReticleDefinition = WeaponInfo->ReticleDefinition;

	float BaseWalkSpeed = mOwner->GetBaseWalkSpeed();

	mOwner->SetDefaultWalkSpeed(BaseWalkSpeed *= (1.0f - WeaponInfo->WeaponWeight / 100.0f));


	if (mWeaponMeshComp)
	{
		switch (mWeaponType)
		{
		case EWeaponType::Unarmed:
			UE_LOG(LogTemp, Warning, TEXT("Unarmed"));
			mWeaponMeshComp->SetWorldScale3D(FVector(1, 1, 1));
			mWeaponMeshComp->SetStaticMesh(nullptr);
			break;

		case EWeaponType::Melee:
			UE_LOG(LogTemp, Warning, TEXT("Melee"));
			mWeaponMeshComp->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));
			mWeaponMeshComp->SetStaticMesh(WeaponInfo->Mesh);
			break;

		case EWeaponType::Ranged:
			UE_LOG(LogTemp, Warning, TEXT("Ranged"));
			mWeaponMeshComp->SetWorldScale3D(FVector(1, 1, 1));
			mWeaponMeshComp->SetStaticMesh(WeaponInfo->Mesh);
			RangedWeaponInfo = WeaponInfo->RangedWeaponInfo;
			mTimeSinceLastShot = 1.f / mFireRate;
			break;
		}
	}

	ChangeWeaponPos(WeaponKey);
}

void UAWeekWeaponComponent::ChangeWeaponPos(FName SocketName)
{
	if (mWeapon && mOwner)
	{
		mWeapon->AttachToComponent(
			mOwner->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			SocketName
		);
	}
}

void UAWeekWeaponComponent::Reload()
{
	int32 BulletUsage = mBulletMaxStack - mCurrentBullet;
	if (mWeaponKey == FName("Rifle") && mOwner->GetPlayerInventoryComponent()->TryRemoveAmountOfItem(FName("test_004"), BulletUsage))
	{
		mCurrentBullet = mBulletMaxStack;
		bOutOfBullet = false;
	}
	else if (mWeaponKey == FName("Pistol") && mOwner->GetPlayerInventoryComponent()->TryRemoveAmountOfItem(FName("test_005"), BulletUsage))
	{
		mCurrentBullet = mBulletMaxStack;
		bOutOfBullet = false;
	}
}

void UAWeekWeaponComponent::TickSpread(float DeltaTime)
{
	if (mTimeSinceLastShot >= RangedWeaponInfo.HeatShotRecoveryDelay)
	{
		const float RecoveryHeat = RangedWeaponInfo.HeatShotRecoveryCurve.GetRichCurveConst()->Eval(RangedWeaponInfo.CurrentHeat);
		const float NewHeat = RangedWeaponInfo.CurrentHeat - (RecoveryHeat * DeltaTime);
		RangedWeaponInfo.CurrentHeat = RangedWeaponInfo.ClampHeatRange(NewHeat);
		RangedWeaponInfo.CurrentSpreadAngle = RangedWeaponInfo.HeatToSpreadCurve.GetRichCurveConst()->Eval(RangedWeaponInfo.CurrentHeat);
	}
}

void UAWeekWeaponComponent::TickMultipliers(float DeltaTime)
{
	if (CharacterMovementComponent == nullptr)
		return;
	
	const float TransitSpeed = RangedWeaponInfo.TransitSpeed;

	float JumpTargetMultiplier = CharacterMovementComponent->IsFalling() == true?RangedWeaponInfo.JumpSpreadMultiplierNormal:1.0f;
	
	float MovementTargetMultiplier = FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, CharacterMovementComponent->GetMaxSpeed()),
		FVector2D(1.0f, RangedWeaponInfo.MoveSpreadMultiplierNormal), mOwner->GetVelocity().Size());

	RangedWeaponInfo.JumpSpreadMultiplier = FMath::FInterpTo(RangedWeaponInfo.JumpSpreadMultiplier, JumpTargetMultiplier, DeltaTime, TransitSpeed);
	RangedWeaponInfo.StandingSpreadMultiplier = FMath::FInterpTo(RangedWeaponInfo.StandingSpreadMultiplier, MovementTargetMultiplier, DeltaTime, TransitSpeed);
	
	APlayerController* PC = Cast<APlayerController>(CharacterMovementComponent->GetCharacterOwner()->GetController());
	
	bool bIsRotating = false;
	if (PC)
	{
		static FRotator PrevRotater = PC->GetControlRotation();
		const FRotator NowRotator = PC->GetControlRotation();
		bIsRotating = !(NowRotator - PrevRotater).IsNearlyZero();
		PrevRotater = NowRotator;
	}
	
	const float LookTargetMultiplier = bIsRotating ? RangedWeaponInfo.LookSpreadMultiplierNormal:1.0f;
	RangedWeaponInfo.LookSpreadMultiplier = FMath::FInterpTo(RangedWeaponInfo.LookSpreadMultiplier, LookTargetMultiplier, DeltaTime, TransitSpeed);
	RangedWeaponInfo.CurrentSpreadMultiplier = FMath::Max(RangedWeaponInfo.LookSpreadMultiplier, RangedWeaponInfo.StandingSpreadMultiplier) * RangedWeaponInfo.JumpSpreadMultiplier;

}

void UAWeekWeaponComponent::AddSpreadHeat()
{
	const float AddHeat = RangedWeaponInfo.HeatShotCurve.GetRichCurveConst()->Eval(RangedWeaponInfo.CurrentHeat);
	RangedWeaponInfo.CurrentHeat = RangedWeaponInfo.ClampHeatRange(RangedWeaponInfo.CurrentHeat + AddHeat);
	RangedWeaponInfo.CurrentSpreadAngle = RangedWeaponInfo.HeatToSpreadCurve.GetRichCurveConst()->Eval(RangedWeaponInfo.CurrentHeat);
}

// AWeekWeaponComponent.cpp

FVector UAWeekWeaponComponent::GetFireDirection()
{
	if (!mOwner)
		return FVector::ForwardVector;

	APlayerController* PC = Cast<APlayerController>(mOwner->GetController());
	if (!PC)
		return FVector::ForwardVector;

	int32 ViewportSizeX, ViewportSizeY;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenCenter(ViewportSizeX / 2.0f, ViewportSizeY / 2.0f);

	FVector CameraLocation, CameraDirection;
	PC->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, CameraLocation, CameraDirection);

	const float FinalSpreadAngle = CalculateFinalSpreadAngle();
	CameraDirection = FRangedWeaponInfo::GetRandomDirectionInCone(CameraDirection, FinalSpreadAngle,  RangedWeaponInfo.CenterBias);
	return CameraDirection;
}

void UAWeekWeaponComponent::Fire()
{
	// ... ź�� üũ ...
	bOutOfBullet = false;
	UE_LOG(LogTemp, Warning, TEXT("Fire called"));
	APlayerController* PC = Cast<APlayerController>(mOwner->GetController());
	if (!PC)
		return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	FVector FireDirection = GetFireDirection();

	// ī�޶� ��ġ���� ���� Ʈ���̽��ؼ� ��ǥ ���� ã��
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (FireDirection * 10000.0f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(mOwner);
	QueryParams.AddIgnoredActor(mWeapon);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	FVector TargetPoint = bHit ? HitResult.Location : TraceEnd;

	FVector MuzzleLocation = GetMuzzleLocation();
	FVector MuzzleToTarget = (TargetPoint - MuzzleLocation).GetSafeNormal();
	FRotator MuzzleRotation = MuzzleToTarget.Rotation();

	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAWeekWeaponProjectile* Bullet = GetWorld()->SpawnActor<AAWeekWeaponProjectile>(
		mProjectile,
		MuzzleLocation,
		MuzzleRotation,
		Param
	);
	Bullet->SetDamage(mDamage);
	mCurrentBullet -= mBulletUsagePerSingle;
	UE_LOG(LogTemp, Warning, TEXT("%d"), mCurrentBullet);

	DrawDebugLine(
		GetWorld(),
		MuzzleLocation,
		TargetPoint,
		FColor::Red,
		false,
		2.0f,
		0,
		2.0f
	);

	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit: %s at %s"),
		//	*HitResult.GetActor()->GetName(),
		//	*HitResult.Location.ToString());

		DrawDebugSphere(
			GetWorld(),
			HitResult.Location,
			10.0f,
			12,
			FColor::Orange,
			false,
			2.0f
		);
	}

	if (mFireEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			mFireEffect,
			MuzzleLocation,
			FireDirection.Rotation(),
			FVector(1.0f)
		);
	}

	if (mCurrentBullet <= 0)
	{
		bOutOfBullet = true;
	}

	AddSpreadHeat();
}
