// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeaponProjectile.h"

#include "GameEventMessageSubsystem.h"
#include "../../System/DamageSystemComponent.h"

// Sets default values
AAWeekWeaponProjectile::AAWeekWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	SetRootComponent(mBody);
	mBody->SetBoxExtent(FVector(15.0, 10.0, 10.0));
	mBody->SetCollisionProfileName(TEXT("PlayerAttack"));

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mMesh->SetupAttachment(mBody);

	mMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	mMovement->SetUpdatedComponent(mBody);
	mMovement->ProjectileGravityScale = 0.f;
	mMovement->InitialSpeed = 1000.f;
	mMovement->OnProjectileStop.AddDynamic(this, &AAWeekWeaponProjectile::ProjectileStop);

}

// Called when the game starts or when spawned
void AAWeekWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	mMovement->Velocity = GetActorForwardVector() * InitialSpeed;
}

// Called every frame
void AAWeekWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Duration > 0)
	{
		Duration-=DeltaTime;
	}

	if (Duration <= 0)
		Destroy();
}

void AAWeekWeaponProjectile::ProjectileStop(const FHitResult& Hit)
{
	// When hit actor implements DamageAble Interface
	UE_LOG(LogTemp, Warning, TEXT("Projectile Hit at %s %f"), *Hit.GetActor()->GetName(), mDamage);
	if (Hit.GetActor()->GetClass()->ImplementsInterface(UDamageAble::StaticClass()))
	{
		FDamageInfo DamageInfo;
		DamageInfo.Amount = mDamage;
		DamageInfo.HitInfo.ImpactPoint = Hit.ImpactPoint;
		DamageInfo.HitInfo.ImpactNormal = Hit.ImpactNormal;
		DamageInfo.HitInfo.BoneName = Hit.BoneName;
		DamageInfo.HitInfo.HitComponent = Hit.GetComponent();
		IDamageAble::Execute_TakeDamage(Hit.GetActor(), DamageInfo);
	}

	if (Hit.GetActor()->ActorHasTag("Enemy"))
	{
		FEmptyPayload EmptyPayload;
		UGameEventMessageSubsystem::Get(this).BroadcastMessage(
	FGameplayTag::RequestGameplayTag(TEXT("Event.Hit.Enemy")), EmptyPayload);
	}

	Destroy();
}

