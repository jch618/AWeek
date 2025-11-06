// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TurretPlacedActor.h"

#include "AWeek/Grid/TurretComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ATurretPlacedActor::ATurretPlacedActor()
{
	YawPivot = CreateDefaultSubobject<USceneComponent>(TEXT("YawPivot"));
	YawPivot->SetupAttachment(BoxMesh ? static_cast<USceneComponent*>(BoxMesh) : Root);

	PitchPivot = CreateDefaultSubobject<USceneComponent>(TEXT("PitchPivot"));
	PitchPivot->SetupAttachment(YawPivot);

	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(PitchPivot);
	TurretHead->SetGenerateOverlapEvents(false);
	TurretHead->SetSimulatePhysics(false);


	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrow"));
	MuzzleArrow->SetupAttachment(TurretHead);

	if (MuzzleArrow && TurretHead)
	{
		MuzzleArrow->AttachToComponent(TurretHead, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Muzzle"));
	}
	
	/*Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(TurretHead);*/

	TurretLogic = CreateDefaultSubobject<UTurretComponent>(TEXT("TurretLogic"));

	SenseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SenseSphere"));
	SenseSphere->SetupAttachment(GetRootComponent()); //일단 root
	SenseSphere->InitSphereRadius(2000.f);

	SenseSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SenseSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	SenseSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SenseSphere->SetGenerateOverlapEvents(true);

	UE_LOG(LogTemp, Log, TEXT("Sense test1"));
	if (TurretLogic)
	{
		UE_LOG(LogTemp, Log, TEXT("Sense test2"));
		//TODO TurretComponent Setting
		TurretLogic->YawPivot = YawPivot;
		TurretLogic->PitchPivot = PitchPivot;
		TurretLogic->Muzzle = MuzzleArrow;

		//TODO TurretComponent Dmg, Bullet type, Rotate Speed Setting
	}
	
	PrimaryActorTick.bCanEverTick = true;
}

void ATurretPlacedActor::BeginPlay()
{
	Super::BeginPlay();
	SenseSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurretPlacedActor::OnSenseBegin);
	SenseSphere->OnComponentEndOverlap.AddDynamic(this, &ATurretPlacedActor::OnSenseEnd);


	YawPivot  ->SetUsingAbsoluteRotation(false);
	PitchPivot->SetUsingAbsoluteRotation(false);
	TurretHead  ->SetUsingAbsoluteRotation(false);
	/*if (TurretLogic)
	{
		UE_LOG(LogTemp, Log, TEXT("Sense test2"));
		//TODO TurretComponent Setting
		TurretLogic->YawPivot = YawPivot;
		TurretLogic->PitchPivot = PitchPivot;
		TurretLogic->Muzzle = MuzzleArrow;

		//TODO TurretComponent Dmg, Bullet type, Rotate Speed Setting
	}*/

	//TODO FIndPlayer
	if (UWorld* World = GetWorld())
	{
		PlayerActor = UGameplayStatics::GetPlayerPawn(World, 0);
	}
}

void ATurretPlacedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LookAtYawOnly(PlayerActor, DeltaTime, YawSpeedDegPerSec);
	/*PickOrValidateTarget();
	AimAtTarget(DeltaTime);*/
}


void ATurretPlacedActor::OnSenseBegin(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp,
									  int32 BodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	
	if (!Other || Other == this) return;
	bActive = true;
	if (OtherComp->GetCollisionProfileName() == TEXT("Player"))//나중에 zombie 넣기
	{
		Targets.Add(OtherComp);
	}
	if (APawn* Pawn = Cast<APawn>(Other))
	{
		//TODO TurretLogic add target and check zombie class
		//TurretLogic->AddTarget(Pawn);
	}
}

void ATurretPlacedActor::OnSenseEnd(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
	bActive = false;
	if (APawn* Pawn = Cast<APawn>(Other))
	{
		
		//TODO TurretLogic remove target and check zombie class
		//TurretLogic->RemoveTarget(Pawn);
	}

	if (OtherComp->GetCollisionProfileName() == TEXT("Player"))//나중에 zombie 넣기
	{
		Targets.Remove(OtherComp);
	}
}

void ATurretPlacedActor::PickOrValidateTarget()
{
	if (APawn* P = CurrentTarget.Get())
	{
		if (!IsValid(P) || P->IsPendingKillPending()) CurrentTarget = nullptr;
	}
}

void ATurretPlacedActor::AimAtTarget(float DT)
{
	
}
float ATurretPlacedActor::StepAngle(float Current, float Target, float SpeedDegPerSec, float DT)
{
	const float Delta = FMath::FindDeltaAngleDegrees(Current, Target);
	const float Step  = SpeedDegPerSec * DT;
	if (FMath::Abs(Delta) <= Step) return Target;
	return Current + FMath::Clamp(Delta, -Step, Step);
}

void ATurretPlacedActor::LookAtYawOnly(AActor* Target, float DeltaTime, float Speed)
{
	if (!YawPivot || !IsValid(Target)) return;
	//if (!bActive)return;
	// Yaw만 보게 하려면, 두 점의 Z를 맞춰서 수평 벡터를 만든다.
	FVector From = YawPivot->GetComponentLocation();
	FVector To   = Target->GetActorLocation();
	To.Z = From.Z;

	const FVector Dir = (To - From).GetSafeNormal();
	if (Dir.IsNearlyZero()) return;

	FRotator TargetRot = Dir.Rotation(); // (= MakeRotFromX)
	TargetRot.Pitch = 0.f;
	TargetRot.Roll  = 0.f;

	const FRotator Current = YawPivot->GetComponentRotation();
	const FRotator NewRot  = FMath::RInterpTo(Current, TargetRot, DeltaTime, Speed);

	YawPivot->SetWorldRotation(NewRot);
}



