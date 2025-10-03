// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeaponComponent.h"
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

void UAWeekWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsFiring && !bIsReloading && mWeaponType == EWeaponType::Ranged)
	{
		mTimeSinceLastShot += DeltaTime;

		// 발사 간격 계산 (RPM -> 초당 발사 간격)
		float FireInterval = 60.0f / mFireRate;

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
	mBulletMaxStack = WeaponInfo->BulletMaxStack;
	mBulletUsagePerSingle = WeaponInfo->BulletUsagePerSingle;
	mFireRate = WeaponInfo->FireRate;
	mFireEffect = WeaponInfo->FireEffect;
	mReticleDefinition = WeaponInfo->ReticleDefinition;
	
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

// AWeekWeaponComponent.cpp

FVector UAWeekWeaponComponent::GetFireDirection()
{
    if (!mOwner)
        return FVector::ForwardVector;

    // 플레이어 카메라 가져오기
    APlayerController* PC = Cast<APlayerController>(mOwner->GetController());
    if (!PC)
        return FVector::ForwardVector;

    // 화면 중앙 좌표
    int32 ViewportSizeX, ViewportSizeY;
    PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
    FVector2D ScreenCenter(ViewportSizeX / 2.0f, ViewportSizeY / 2.0f);

    // 화면 중앙에서 월드 공간으로 변환
    FVector WorldLocation, WorldDirection;
    PC->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldLocation, WorldDirection);

    return WorldDirection;
}

void UAWeekWeaponComponent::Fire()
{
    if (bIsReloading)
        return;
    if (mCurrentBullet < mBulletUsagePerSingle)
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo! Reloading..."));
        //Reload();
        return;
    }

    mCurrentBullet -= mBulletUsagePerSingle;

    // === 총알 발사 로직 ===
    FVector MuzzleLocation = GetMuzzleLocation();
    FVector FireDirection = GetFireDirection();

    // Line Trace로 맞은 위치 확인
    FVector TraceStart = MuzzleLocation;
    FVector TraceEnd = TraceStart + (FireDirection * 10000.0f); // 10000 유닛 거리

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(mOwner);
    QueryParams.AddIgnoredActor(mWeapon);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_EngineTraceChannel5,
        QueryParams
    );

    // 디버그 라인 그리기 (개발용)
    DrawDebugLine(
        GetWorld(),
        TraceStart,
        bHit ? HitResult.Location : TraceEnd,
        bHit ? FColor::Red : FColor::Green,
        false,
        2.0f,
        0,
        2.0f
    );

    if (bHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit: %s at %s"),
            *HitResult.GetActor()->GetName(),
            *HitResult.Location.ToString());

        // TODO: 데미지 처리
        // TODO: 피격 이펙트 생성

        // 임시 이펙트 (구체 그리기)
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

	FVector	MuzzleLoctaion = GetMuzzleLocation();

	if (mFireEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			mFireEffect,
			MuzzleLoctaion,
			FireDirection.Rotation(),
			FVector(1.0f)
		);
	}

	/*
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AAWeekWeaponProjectile* Bullet = GetWorld()->SpawnActor<AAWeekWeaponProjectile>(MuzzleLoctaion, FireDirection.Rotation(), Param);
	Bullet->SetDamage(mDamage);
	*/

    if (mCurrentBullet <= 0)
    {
        //Reload();
    }
}