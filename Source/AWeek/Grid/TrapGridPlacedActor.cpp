// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TrapGridPlacedActor.h"

ATrapGridPlacedActor::ATrapGridPlacedActor()
{
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(Root);

	DamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATrapGridPlacedActor::BeginPlay()
{
	Super::BeginPlay();


	if (DamageBox)
	{
		DamageBox->SetHiddenInGame(false);
		DamageBox->SetVisibility(false);

		DamageBox->OnComponentBeginOverlap.AddDynamic(
			this, &ATrapGridPlacedActor::OnDamageBoxBeginOverlap);

		DamageBox->OnComponentEndOverlap.AddDynamic(
			this, &ATrapGridPlacedActor::OnDamageBoxEndOverlap);
	}

	GetWorldTimerManager().SetTimer(
		FlameDamageTimerhandle,
		this,
		&ATrapGridPlacedActor::DamageBoxTimer,
		DamageInterval,
		true  // 반복
	);
}

void ATrapGridPlacedActor::DamageBoxTimer()
{
	if (ActorsInDamageBox.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[DamageBox] 현재 들어온 Actor 없음"));
		return;
	}

	int32 Index = 0;
	UE_LOG(LogTemp, Log, TEXT("[DamageBox] 현재 들어온 Actor 목록 (%d개):"), ActorsInDamageBox.Num());

	for (AActor* Target : ActorsInDamageBox)
	{
		if (!IsValid(Target)) continue; // 이미 Destroy된 애들 방어

		if (!Target->GetClass()->ImplementsInterface(UDamageAble::StaticClass())) //해당 인터페이스 없는 객체 방어
			continue;

		UE_LOG(LogTemp, Log, TEXT("  [%d] %s"),
			Index,
			*Target->GetName());

		++Index;

		FDamageInfo DamageInfo;
		DamageInfo.Amount = DamagePerTick;
		//DamageInfo.DamageType = EDamageType::Explosion;          
		DamageInfo.DamageResponse = EDamageResponse::HitReaction;
		DamageInfo.CanBeBlocked = false;
		DamageInfo.CanBeParried = false;
		DamageInfo.ShouldForceInterrupt = false;
		DamageInfo.ShouldDamageInvincible = false;

		
		/*const FVector ImpactPoint  = Target->GetActorLocation();
		const FVector ImpactNormal = -YawPivot->GetForwardVector();

		DamageInfo.HitInfo.ImpactPoint  = ImpactPoint;
		DamageInfo.HitInfo.ImpactNormal = ImpactNormal;
		DamageInfo.HitInfo.HitComponent = Cast<UPrimitiveComponent>(Target->GetRootComponent());
		DamageInfo.HitInfo.BoneName     = NAME_None;*/

		IDamageAble::Execute_TakeDamage(Target, DamageInfo);
	}
}


void ATrapGridPlacedActor::OnDamageBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("a: OtherActor=%s IsValid=%d IsSelf=%d"),
	*GetNameSafe(OtherActor),
	IsValid(OtherActor) ? 1 : 0,
	OtherActor == this ? 1 : 0
);
	if (!IsValid(OtherActor) || OtherActor == this) return;

	
	if (TargetClass && !OtherActor->IsA(TargetClass))
	{
		UE_LOG(LogTemp,Log,TEXT("c"));
		return; 
	}
	UE_LOG(LogTemp,Log,TEXT("d"));
	ActorsInDamageBox.Add(OtherActor);

	UE_LOG(LogTemp, Log, TEXT("Enter DamageBox: %s (현재 %d개)"),
		*OtherActor->GetName(),
		ActorsInDamageBox.Num());
}

void ATrapGridPlacedActor::OnDamageBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor)) return;


	if (TargetClass && !OtherActor->IsA(TargetClass))
	{
		return;
	}

	if (ActorsInDamageBox.Remove(OtherActor) > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Leave DamageBox: %s (현재 %d개)"),
			*OtherActor->GetName(),
			ActorsInDamageBox.Num());
	}
}





