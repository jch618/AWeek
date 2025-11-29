// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "../Data/AWeekPlayerAnimInfo.h"
#include "../Character/AWeekPlayerCharacter.h"
#include "../AWeekAssetManager.h"
#include "KismetAnimationLibrary.h"
#include "../Character/AWeekPlayerCharacter.h"

void UAWeekPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	mOwner = Cast<AAWeekPlayerCharacter>(GetOwningActor());

	// �ִϸ��̼� ���������̺� ��ü�� ������
	if (!AnimInfoDT) return;

	// ���������̺��� RowMap�̶�� TMap������ ��ȯ�� (Row�� Ű��, �� ����� ���� ����)
	TMap<FName, uint8*> RowMap = AnimInfoDT->GetRowMap();
	for (auto& Row : RowMap)
	{
		FName RowKey = Row.Key;

		// uint8*(���������̺� Value�� ����Ű�� ������)�� FPlayerInfo�� ĳ��������
		// Cast�� �𸮾� Ŭ���� �����̶��, reinterperet_cast�� C++ ǥ�� ĳ��Ʈ �Լ��� ����ü�� ��ȯ���ش�.
		FPlayerAnimInfo* RowData = reinterpret_cast<FPlayerAnimInfo*>(Row.Value);

		// ���� ��ȿ�ϴٸ� AnimInstance�� ����ִ� �ʿ� �߰����ش�.
		if (RowData)
		{
			mAnimMap.Add(RowKey, *RowData);
		}
	}

	// �ϼ��� mAnimMap�� �÷��̾��� ����(���/��������������/���Ÿ�����������)�� ��Ÿ���� FName�� Ű������ FPlayerAnimInfo�� ����ְ�,
	// �̰Ÿ� �ִ��ν��Ͻ��� �ִϸ��̼� ������, �����彺���̽�, ��Ÿ�ַ� �Ҵ�����.
	// �� ���� ���۽� �÷��̾��� ��� �ִϸ��̼��� �������� �� ���۷����� �ִ��ν��Ͻ��� �� ����ֵ��� �Ѵ�.
	// �÷��̾��� ���¸� Ű ������ ���� �Ѱ��ָ� �ִϸ��̼� �������̵� ����
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
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, mOwner->GetActorRotation());
	
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

void UAWeekPlayerAnimInstance::PlayMontageByName(FName Name, float PlayRate)
{
	UAnimMontage* Montage = FindAnimMontage(Name);
	if (Montage)
	{
		mOwner->EndFire();
		Montage_Play(Montage, PlayRate);
	}
}

void UAWeekPlayerAnimInstance::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == FindAnimMontage(TEXT("Vault")))
	{
		mOwner->VaultEnd();
	}

	if (Montage == FindAnimMontage(TEXT("Climb")))
	{
		mOwner->ClimbEnd();
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
