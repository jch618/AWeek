// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPakourComponent.h"
#include "../AWeekCharacter.h"

// Sets default values for this component's properties
UAWeekPakourComponent::UAWeekPakourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAWeekPakourComponent::BeginPlay()
{
	Super::BeginPlay();
	mOwner = Cast<AAWeekCharacter>(GetOwner());

	if (mOwner)
	{
		mOwnerMWC = mOwner->GetMWC();
	}
	// ...
}


// Called every frame
void UAWeekPakourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAWeekPakourComponent::TriggerPakour(EPakourType PakourType)
{
	if (!bCanPakour)
		return false;

	mPakourType = PakourType;

	FHitResult WallHit;
	switch (mPakourType)
	{
	case EPakourType::Vault:
		WallHit = DetectWall(200);
		break;
	case EPakourType::Ledge:
		WallHit = DetectWall(140);
		break;
	}
	
	if (WallHit.bBlockingHit)
		return ScanWall(WallHit);
	return false;
}

FHitResult UAWeekPakourComponent::DetectWall(float Distance)
{
	FHitResult WallHit;

	for(int i=0; i<7; i++)
	{
		FVector SphereLoc = mOwner->GetActorLocation();
		SphereLoc.Z += (-60 + i*20);
		FVector Start = SphereLoc - (mOwner->GetActorForwardVector() * 20);
		FVector End = SphereLoc + (mOwner->GetActorForwardVector() * Distance);

		WallHit = WallTracing(ETraceType::Sphere, Start, End);

		if(WallHit.bBlockingHit)
			break;
	}
	return WallHit;
}

bool UAWeekPakourComponent::ScanWall(FHitResult Hit)
{
	// 1: ���� �����Ǿ����� 3���� ������ 10��ġ�� �������鼭 �� ���κ��� Ž�� (����Ʈ���̽�)
	FVector HitLocation = Hit.Location;
	HitLocation.Z += 300;
	for (int i = 0; i < 30; i++)
	{
		FVector Start = HitLocation - (-Hit.Normal.GetSafeNormal() * 20);
		Start.Z -= i*10;
		FVector End = HitLocation + (-Hit.Normal.GetSafeNormal() * 80);
		End.Z -= i*10;

		mFirstWallHit = WallTracing(ETraceType::Line, Start, End, FColor::Yellow);

		if (mFirstWallHit.bBlockingHit)
			break;
	}

	if (!mFirstWallHit.bBlockingHit)
		return false;

	// 2: �� �β��� Ž���ϸ鼭 ó�� �����κа� �������� ���� �κ� Ž��
	mWallRotation = -mFirstWallHit.Normal.GetSafeNormal();
	for (int i = 0; i < 10; i++)
	{
		// ó�� ���� ��ġ���� ������ 20��Ƽ���;� �÷������鼭 Ž���Ѵ�
		FVector Start = mFirstWallHit.Location + mWallRotation * 20*i;
		FVector End = mFirstWallHit.Location + mWallRotation * 20*i;
		Start.Z += 10;

		FHitResult HitResult = WallTracing(ETraceType::Sphere, Start, End, FColor::Cyan);

		if (HitResult.bBlockingHit)
		{
			if (i == 0)
			{
				mFirstTopHit = HitResult;
			}
			else
			{
				mLastTopHit = HitResult;
			}
		}
	}

	if (!mLastTopHit.bBlockingHit)
		return false;

	if (mPakourType == EPakourType::Ledge)
		return TryLedge();

	// 3: �� �β� ���κ��� ImpactPoint�� ���ͼ� ���� ���κ��� Ž���Ѵ�.
	mEndOfWallHit = WallTracing(ETraceType::Sphere,
		mLastTopHit.ImpactPoint + mWallRotation * 20,
		mLastTopHit.ImpactPoint, FColor::Yellow);

	if (!mEndOfWallHit.bBlockingHit)
		return false;

	// 4: �� ���κп��� �÷��̾��� Ű��ŭ ������ Ž���ϸ鼭 ��Ʈ ���� ��ġ�� ��´�.
	FVector Start = mEndOfWallHit.ImpactPoint + mWallRotation * 60;
	FVector End = mEndOfWallHit.ImpactPoint + mWallRotation * 60;
	float Height = mOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.0f;
	End.Z -= Height;
	mVaultLandHit = WallTracing(ETraceType::Sphere,
		Start,
		End, FColor::Red);

	if (mPakourType==EPakourType::Vault && mVaultLandHit.bBlockingHit)
		return TryVault();

	return false;
}

bool UAWeekPakourComponent::TryVault()
{
	float GroundHeight = mOwner->GetMesh()->GetComponentLocation().Z;
	float WallHeight = mFirstWallHit.Location.Z - GroundHeight;

	if (WallHeight > 40 && WallHeight < 90)
	{
		SetVaultMotionWarping();
		mOwner->VaultStart();
		return true;
	}

	return false;
}

bool UAWeekPakourComponent::TryLedge()
{
	float GroundHeight = mOwner->GetMesh()->GetComponentLocation().Z;
	float WallHeight = mFirstWallHit.Location.Z - GroundHeight;
	float PlayerHeight = 190.f;

	if (FVector::Dist(mFirstTopHit.Location, mLastTopHit.Location) > 60 &&
		WallHeight >= 220 && WallHeight <= 300)
	{
		SetLedgeMotionWarping(PlayerHeight);
		SetClimbMotionWarping();
		mOwner->LedgeStart();
		return true;
	}

	return false;
}

void UAWeekPakourComponent::SetVaultMotionWarping()
{
	FVector Start = mFirstTopHit.Location;
	Start.Z -= 70;
	FVector End = mVaultLandHit.Location;

	mOwnerMWC->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("VaultStart"),
		Start,
		mWallRotation.Rotation()
	);

	mOwnerMWC->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("VaultEnd"),
		End,
		mWallRotation.Rotation()
	);
}

void UAWeekPakourComponent::SetLedgeMotionWarping(float PlayerHeight)
{
	FVector Dest = mFirstTopHit.ImpactPoint - mOwner->GetActorForwardVector()*50;

	Dest.Z-=PlayerHeight;

	mOwnerMWC->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("Ledge"),
		Dest,
		mWallRotation.Rotation()
	);
}

void UAWeekPakourComponent::SetClimbMotionWarping()
{
	FVector Dest = mFirstWallHit.ImpactPoint;
	mOwnerMWC->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("Climb"),
		Dest,
		mWallRotation.Rotation()
	);
}
