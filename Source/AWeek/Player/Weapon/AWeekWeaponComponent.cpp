// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeaponComponent.h"
#include "../../AWeekAssetManager.h"

// Sets default values for this component's properties
UAWeekWeaponComponent::UAWeekWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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
			FName("Weapon")
		);
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
	mWeaponType = WeaponInfo->WeaponType;
	mDamage = WeaponInfo->Damage;
	mBulletMaxStack = WeaponInfo->BulletMaxStack;
	mBulletUsagePerSingle = WeaponInfo->BulletUsagePerSingle;
	mReticleDefinition = WeaponInfo->ReticleDefinition;
	
	if (!IsValid(Mesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Mesh is nullptr"));
		return;
	}

	if (mWeaponMeshComp)
	{
		mWeaponMeshComp->SetRelativeScale3D(FVector(1, 1, 1));
		mWeaponMeshComp->SetRelativeLocation(FVector(0, 0, 0));
		mWeaponMeshComp->SetRelativeRotation(FRotator::ZeroRotator);

		switch (mWeaponType)
		{
		case EWeaponType::Unarmed:
			break;

		case EWeaponType::Melee:
			mWeaponMeshComp->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
			mWeaponMeshComp->SetRelativeLocation(FVector(0, 0, 40));
			mWeaponMeshComp->SetRelativeRotation(FRotator(90, 0, -40));
			break;

		case EWeaponType::Ranged:
			break;
		}
		mWeaponMeshComp->SetStaticMesh(WeaponInfo->Mesh);
	}



}
