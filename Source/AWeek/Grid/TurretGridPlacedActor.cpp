// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TurretGridPlacedActor.h"

#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "AWeek/System/DamageInfo.h"
#include "AWeek/System/DamageHitInfo.h"
#include "AWeek/System/IDamageAble.h"

static constexpr ECollisionChannel ECC_GunTrace = ECC_GameTraceChannel13;

ATurretGridPlacedActor::ATurretGridPlacedActor()
{
	PrimaryActorTick.bCanEverTick = true;


	YawPivot = CreateDefaultSubobject<USceneComponent>(TEXT("YawPivot"));
	YawPivot->SetupAttachment(Root);
	
	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(YawPivot);

	DetectCapsule = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCapsule"));
	DetectCapsule->SetupAttachment(Root);
	DetectCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATurretGridPlacedActor::BeginPlay()
{
	Super::BeginPlay();
	if (YawPivot && GetWorld())
	{
		// 0.5초마다 TurretLineTrace 실행
		GetWorldTimerManager().SetTimer(
			TraceTimerHandle,
			this,
			&ATurretGridPlacedActor::TurretLineTrace,
			TraceInterval,
			true   // 반복
		);
	}

	if (DetectCapsule)
	{
		DetectCapsule->SetHiddenInGame(false);
		DetectCapsule->SetVisibility(true);
	}
}


void ATurretGridPlacedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//지금 매 tick 마다 Overlap 확인하는 코드임
	//최적화 ㄱㅊ?
	TArray<AActor*> Overlaps;
	if (TargetClass)
		DetectCapsule->GetOverlappingActors(Overlaps, TargetClass);
	else
		DetectCapsule->GetOverlappingActors(Overlaps);

	if (Overlaps.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Turret overlap is none"));
		return;
	} 
	UE_LOG(LogTemp, Log, TEXT("Turret overlap is %d"), Overlaps.Num());
	// 가장 가까운 타겟 찾기 -> TODO첫 타겟 계속 쫒아가기
	AActor* BestTarget = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();
	const FVector MyLoc = YawPivot->GetComponentLocation();

	for (AActor* T : Overlaps)
	{
		if (!IsValid(T) || T == this) continue;
		float DistSq = FVector::DistSquared(MyLoc, T->GetActorLocation());
		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			BestTarget = T;
		}
	}
	if (!BestTarget) return;

	FVector ToTarget = BestTarget->GetActorLocation() - MyLoc;
	if (bYawOnly) ToTarget.Z = 0.f;
	if (ToTarget.IsNearlyZero()) return;

	FRotator TargetRot = ToTarget.Rotation();
	FRotator CurrentRot = YawPivot->GetComponentRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, RotateSpeed);

	if (bYawOnly)
		NewRot.Pitch = CurrentRot.Pitch;

	YawPivot->SetWorldRotation(NewRot);
}

void ATurretGridPlacedActor::TurretLineTrace()
{
	UWorld* World = GetWorld();
    if (!World || !YawPivot)
    {
        return;
    }

    const FVector Start   = YawPivot->GetComponentLocation();
    const FVector Forward = YawPivot->GetForwardVector();
    const FVector End     = Start + Forward * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(TurretTrace), false, this);
    Params.bReturnPhysicalMaterial = false;
    Params.AddIgnoredActor(this);

    const bool bHit = World->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_GunTrace,
        Params
    );

    const FColor LineColor = bHit ? FColor::Red : FColor::Green;
    DrawDebugLine(World, Start, End, LineColor, false, 0.5f, 0, 2.f);

    if (!bHit)
    {
        UE_LOG(LogTemp, Log, TEXT("[Turret Trace] No Hit"));
        return;
    }

    AActor* HitActor = Hit.GetActor();
    if (!IsValid(HitActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("[Turret Trace] Hit but Actor invalid"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Turret Trace] Hit Actor: %s"), *HitActor->GetName());

    // 🔹 1) TargetClass 통과 여부
    if (TargetClass && !HitActor->IsA(TargetClass))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[Turret Trace] %s 는 TargetClass(%s)가 아님 → 스킵"),
            *HitActor->GetName(),
            *GetNameSafe(TargetClass));
        return;
    }

    // 🔹 2) DamageAble 인터페이스 여부
    if (!HitActor->GetClass()->ImplementsInterface(UDamageAble::StaticClass()))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[Turret Trace] %s 는 DamageAble 인터페이스 없음 → 스킵"),
            *HitActor->GetName());
        return;
    }

    UPrimitiveComponent* HitComp = Hit.GetComponent();
    if (!IsValid(HitComp))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("[Turret Trace] HitComponent is null for %s → 스킵"),
            *HitActor->GetName());
        return;
    }

    // 🔹 3) 여기까지 들어오면 진짜 데미지 줌
    FDamageInfo DamageInfo;
    DamageInfo.Amount = DamagePerTick;
    DamageInfo.DamageResponse = EDamageResponse::HitReaction;
    DamageInfo.CanBeBlocked = true;
    DamageInfo.CanBeParried = false;
    DamageInfo.ShouldForceInterrupt = false;
    DamageInfo.ShouldDamageInvincible = false;

    DamageInfo.HitInfo.ImpactPoint  = Hit.ImpactPoint;
    DamageInfo.HitInfo.ImpactNormal = Hit.ImpactNormal;
    DamageInfo.HitInfo.BoneName     = Hit.BoneName;
    DamageInfo.HitInfo.HitComponent = HitComp;

    UE_LOG(LogTemp, Log,
        TEXT("[Turret Trace] Apply Damage %.1f to %s"),
        DamageInfo.Amount,
        *HitActor->GetName());

    IDamageAble::Execute_TakeDamage(HitActor, DamageInfo);
	
	/*UWorld* World = GetWorld();
    if (!World || !YawPivot)
    {
        return;
    }

    const FVector Start   = YawPivot->GetComponentLocation();
    const FVector Forward = YawPivot->GetForwardVector();
    const FVector End     = Start + Forward * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(TurretTrace), false, this);
    Params.bReturnPhysicalMaterial = false;
    Params.AddIgnoredActor(this);

    const bool bHit = World->LineTraceSingleByChannel(
        Hit,
        Start,
        End,
        ECC_GunTrace,
        Params
    );

    const FColor LineColor = bHit ? FColor::Red : FColor::Green;
    DrawDebugLine(World, Start, End, LineColor, false, 0.5f, 0, 2.f);

    // 🔹 1) 아예 안 맞았으면 여기서 끝
    if (!bHit)
    {
        //UE_LOG(LogTemp, Log, TEXT("[Turret Trace] No Hit"));
        return;
    }

    AActor* HitActor = Hit.GetActor();

    // 🔹 2) Actor가 없거나 이미 날아간 경우 방어
    if (!IsValid(HitActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("[Turret Trace] Hit but Actor invalid"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[Turret Trace] Hit Actor: %s"), *HitActor->GetName());

    // 🔹 3) TargetClass 필터
    if (TargetClass && !HitActor->IsA(TargetClass))
    {
        return;
    }

    // 🔹 4) DamageAble 인터페이스 체크
    if (!HitActor->GetClass()->ImplementsInterface(UDamageAble::StaticClass()))
    {
        return;
    }

    // 🔹 5) HitComponent도 널 가능성 있으니 방어
    UPrimitiveComponent* HitComp = Hit.GetComponent();
    if (!IsValid(HitComp))
    {
        UE_LOG(LogTemp, Warning, TEXT("[Turret Trace] HitComponent is null for %s"), *HitActor->GetName());
        return;
    }

    // 🔹 6) 데미지 세팅
    FDamageInfo DamageInfo;
    DamageInfo.Amount = DamagePerTick; // 이름은 Shot이면 DamagePerShot로 바꿔도 됨
    //DamageInfo.DamageType = EDamageType::Projectile;
    DamageInfo.DamageResponse = EDamageResponse::HitReaction;
    DamageInfo.CanBeBlocked = true;
    DamageInfo.CanBeParried = false;
    DamageInfo.ShouldForceInterrupt = false;
    DamageInfo.ShouldDamageInvincible = false;

    DamageInfo.HitInfo.ImpactPoint  = Hit.ImpactPoint;
    DamageInfo.HitInfo.ImpactNormal = Hit.ImpactNormal;
    DamageInfo.HitInfo.BoneName     = Hit.BoneName;
    DamageInfo.HitInfo.HitComponent = HitComp;

    // 🔹 7) 실제 데미지 전달
    IDamageAble::Execute_TakeDamage(HitActor, DamageInfo);*/
}
