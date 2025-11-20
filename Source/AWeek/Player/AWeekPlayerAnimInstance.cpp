// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerAnimInstance.h"
#include "../Data/AWeekPlayerAnimInfo.h"
#include "../Character/AWeekPlayerCharacter.h"
#include "../AWeekAssetManager.h"
#include "../Character/AWeekPlayerCharacter.h"

void UAWeekPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	mOwner = Cast<AAWeekPlayerCharacter>(GetOwningActor());

	// 애니메이션 데이터테이블 전체를 갖고옴
	UDataTable* AnimInfoDT = UAWeekAssetManager::Get().FindDataTable(TEXT("DT_PlayerAnimInfo"));
	if (!AnimInfoDT) return;

	// 데이터테이블을 RowMap이라는 TMap구조로 변환함 (Row의 키값, 각 밸류에 접근 가능)
	TMap<FName, uint8*> RowMap = AnimInfoDT->GetRowMap();
	for (auto& Row : RowMap)
	{
		FName RowKey = Row.Key;

		// uint8*(데이터테이블 Value를 가리키는 포인터)을 FPlayerInfo로 캐스팅해줌
		// Cast는 언리얼 클래스 전용이라면, reinterperet_cast는 C++ 표준 캐스트 함수로 구조체를 변환해준다.
		FPlayerAnimInfo* RowData = reinterpret_cast<FPlayerAnimInfo*>(Row.Value);

		// 값이 유효하다면 AnimInstance가 들고있는 맵에 추가해준다.
		if (RowData)
		{
			mAnimMap.Add(RowKey, *RowData);
		}
	}

	// 완성된 mAnimMap은 플레이어의 상태(빈손/근접무기장착중/원거리무기장착중)를 나타내는 FName을 키값으로 FPlayerAnimInfo를 들고있고,
	// 이거를 애님인스턴스의 애니메이션 시퀀스, 블렌드스페이스, 몽타주로 할당해줌.
	// 즉 게임 시작시 플레이어의 모든 애니메이션을 갖고오고 이 레퍼런스를 애님인스턴스가 다 들고있도록 한다.
	// 플레이어의 상태만 키 값으로 딸깍 넘겨주면 애니메이션 오버라이딩 가능
	ChangeAnimOverride(mStatusKey);
	OnMontageEnded.AddDynamic(this, &UAWeekPlayerAnimInstance::MontageEnd);
}

void UAWeekPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAWeekPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!mOwner)
		return;
	AController* Controller = mOwner->GetController();
	if (!Controller) return;

	FVector Velocity = mOwner->GetVelocity();
	Velocity.Z = 0.0f;
	float Speed = Velocity.Size();
	Direction = CalculateDirection(Velocity, mOwner->GetActorRotation());
	
	FRotator ControlRot = Controller->GetControlRotation();
	ControllerYaw = ControlRot.Yaw;
	ControllerPitch = ControlRot.Pitch;

	float LeftFoot = GetCurveValue(TEXT("Foot_L"));
	float RightFoot = GetCurveValue(TEXT("Foot_R"));

	if (LeftFoot > 0.9f && !bLeftFootTriggered)
	{
		mOwner->FootStepEffect(FName("foot_l_Socket"));
		bLeftFootTriggered = true;
	}
	else if (LeftFoot < 0.1f)
	{
		bLeftFootTriggered = false;
	}

	if (RightFoot > 0.9f && !bRightFootTriggered)
	{
		mOwner->FootStepEffect(FName("foot_r_Socket"));
		bRightFootTriggered = true;
	}
	else if (RightFoot < 0.1f)
	{
		bRightFootTriggered = false;
	}
}

UAnimSequence* UAWeekPlayerAnimInstance::FindAnimSequence(const FName& Name)
{
	TObjectPtr<UAnimSequence>* Sequence = mSequenceMap.Find(Name);

	if (!Sequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sequence %s is not found"), *Name.ToString());
		return nullptr;
	}

	return Sequence->Get();
}

UBlendSpace* UAWeekPlayerAnimInstance::FindBlendSpace(const FName& Name)
{
	TObjectPtr<UBlendSpace>* BlendSpace = mBlendSpaceMap.Find(Name);

	if (!BlendSpace)
	{
		UE_LOG(LogTemp, Warning, TEXT("BS %s is not found"), *Name.ToString());
		return nullptr;
	}

	return BlendSpace->Get();
}

UAnimMontage* UAWeekPlayerAnimInstance::FindAnimMontage(const FName& Name)
{
	TObjectPtr<UAnimMontage>* Montage = mMontageMap.Find(Name);

	if (!Montage)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Montage %s is not found"), *Name.ToString());
		return nullptr;
	}

	return Montage->Get();
}

void UAWeekPlayerAnimInstance::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == FindAnimMontage(TEXT("Vault")))
	{
		mOwner->VaultEnd();
	}

	if (Montage == FindAnimMontage(TEXT("Ledge")))
	{
		mOwner->LedgeEnd();
	}

	if (Montage == FindAnimMontage(TEXT("Attack")))
	{
		mOwner->SetCombatBool(false);
	}
}

void UAWeekPlayerAnimInstance::AnimNotify_MeeleAttack()
{
	mOwner->AttackImpact();
}

void UAWeekPlayerAnimInstance::AnimNotify_Reload()
{
	mOwner->WeaponReload();
}

void UAWeekPlayerAnimInstance::AnimNotify_Heal()
{
	mOwner->Heal();
}

void UAWeekPlayerAnimInstance::AnimNotify_OnDie()
{
	mOwner->GameOver();
	Montage_Pause();
}

void UAWeekPlayerAnimInstance::AnimNotify_AN_FootPlant_Right()
{
	mOwner->FootStepEffect(FName("foot_r_Socket"));
}

void UAWeekPlayerAnimInstance::AnimNotify_AN_FootPlant_Left()
{
	mOwner->FootStepEffect(FName("foot_l_Socket"));
}
