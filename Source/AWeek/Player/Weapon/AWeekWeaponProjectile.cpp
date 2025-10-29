// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeaponProjectile.h"
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
	mMesh->SetupAttachment(mBody);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ThirdParty/base_ballbat/source/BaseBallbat.BaseBallbat'"));
	if (MeshAsset.Succeeded())
		mMesh->SetStaticMesh(MeshAsset.Object);
	mMesh->SetRelativeScale3D(FVector(0.5f));

	mMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	mMovement->SetUpdatedComponent(mBody);
	mMovement->ProjectileGravityScale = 0.f;
	mMovement->InitialSpeed = 150000.f;
	mMovement->OnProjectileStop.AddDynamic(this, &AAWeekWeaponProjectile::ProjectileStop);

}

// Called when the game starts or when spawned
void AAWeekWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
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
	UE_LOG(LogTemp, Warning, TEXT("Hit"));
	if (Hit.GetActor()->GetClass()->ImplementsInterface(UDamageAble::StaticClass()))
	{
		FDamageInfo DamageInfo;
		DamageInfo.Amount = mDamage;
		IDamageAble::Execute_TakeDamage(Hit.GetActor(), DamageInfo);
	}

	Destroy();
}

