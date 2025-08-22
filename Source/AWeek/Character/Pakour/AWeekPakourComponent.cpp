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

	// ...
	
}


// Called every frame
void UAWeekPakourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAWeekPakourComponent::TriggerPakour()
{
	if (!bCanPakour)
		return;

	FHitResult WallHit = DetectWall();
	if (WallHit.bBlockingHit)
		ScanWall(WallHit);
}

FHitResult UAWeekPakourComponent::DetectWall()
{
	FHitResult WallHit;

	for(int i=0; i<7; i++)
	{
		FVector SphereLoc = GetOwner()->GetActorLocation();
		SphereLoc.Z += (-60 + i*20);
		FVector Start = SphereLoc - (GetOwner()->GetActorForwardVector() * 20); 
		FVector End = SphereLoc + (GetOwner()->GetActorForwardVector() * 200); 

		WallHit = WallTracing(ETraceType::Sphere, Start, End);

		if(WallHit.bBlockingHit)
			break;
	}
	return WallHit;
}

void UAWeekPakourComponent::ScanWall(FHitResult Hit)
{
	// 1: 벽이 감지되었을때 3미터 위에서 10센치씩 내려오면서 벽 윗부분을 탐색 (라인트레이싱)
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
		return;

	MeasureWall();

	// 2: 벽 두께를 탐색하면서 처음 맞은부분과 마지막에 맞은 부분 탐색
	FVector WallRotation = -mFirstWallHit.Normal.GetSafeNormal();
	FVector WallLocation = mFirstWallHit.Location;
	for (int i = 0; i < 10; i++)
	{
		// 처음 맞은 위치에서 앞으로 20센티미터씩 늘려나가면서 탐색한다
		FVector Start = WallLocation + WallRotation * 20*i;
		FVector End = WallLocation + WallRotation * 20*i;
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
		return;

	// 3: 벽 두께 끝부분의 ImpactPoint를 얻어와서 벽의 끝부분을 탐색한다.
	mEndOfWallHit = WallTracing(ETraceType::Sphere,
		mLastTopHit.ImpactPoint + WallRotation * 20,
		mLastTopHit.ImpactPoint, FColor::Yellow);

	if (!mEndOfWallHit.bBlockingHit)
		return;

	// 4: 벽 끝부분에서 플레이어의 키만큼 밑으로 탐색하면서 볼트 착지 위치를 얻는다.
	FVector Start = mEndOfWallHit.ImpactPoint + WallRotation * 60;
	FVector End = mEndOfWallHit.ImpactPoint + WallRotation * 60;
	End.Z -= 180;
	mVaultHit = WallTracing(ETraceType::Sphere,
		Start,
		End, FColor::Red);
}

void UAWeekPakourComponent::MeasureWall()
{
	AAWeekCharacter* Chara = Cast<AAWeekCharacter>(GetOwner());
	float GroundHeight = Chara->GetMesh()->GetComponentLocation().Z;

	if (mFirstWallHit.bBlockingHit)
		mDetectedWallHeight = mFirstWallHit.Location.Z - GroundHeight;

	if (mDetectedWallHeight > 90)
	{
		UE_LOG(LogTemp, Warning, TEXT("To High"));
	}
	else
	{
		TryVault();
	}
}

void UAWeekPakourComponent::TryVault()
{
	AAWeekCharacter* Chara = Cast<AAWeekCharacter>(GetOwner());
	Chara->VaultStart();
}

