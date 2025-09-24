// Fill out your copyright notice in the Description page of Project Settings.

#include "AWeekCharacterAnimInstance.h"
#include "../AWeekAssetManager.h"


void UAWeekCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	InitializeAnimMap();
}

void UAWeekCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAWeekCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UAWeekCharacterAnimInstance::InitializeAnimMap()
{
	if (mAnimInfoDT == nullptr) return;

	TMap<FName, uint8*> RowMap = mAnimInfoDT -> GetRowMap();
	for (auto& Row : RowMap)
	{
		FName RowKey = Row.Key;

		FAWeekCharacterAnimInfo* RowData = reinterpret_cast<FAWeekCharacterAnimInfo*>(Row.Value);

		if (RowData)
		{
			mAnimMap.Add(RowKey, *RowData);
		}
	}

	ChangeAnimOverride(mStatusKey);
}

UAnimSequence* UAWeekCharacterAnimInstance::FindAnimSequence(const FName& Name)
{
	TObjectPtr<UAnimSequence>* Sequence = mSequenceMap.Find(Name);

	return Sequence->Get();
}

UBlendSpace* UAWeekCharacterAnimInstance::FindBlendSpace(const FName& Name)
{
	TObjectPtr<UBlendSpace>* BlendSpace = mBlendSpaceMap.Find(Name);

	return BlendSpace->Get();
}

UAnimMontage* UAWeekCharacterAnimInstance::FindAnimMontage(const FName& Name)
{
	TObjectPtr<UAnimMontage>* Montage = mMontageMap.Find(Name);

	return Montage->Get();
}

UAnimMontage* UAWeekCharacterAnimInstance::FindRandomInMontageArray(const FName& Name)
{
	if (FAnimMontageArray* Array = mMontageArrayMap.Find(Name))
	{
		if (Array->Montages.Num() > 0)
		{
			int32 RandIndex = FMath::RandRange(0, Array->Montages.Num() - 1);
			return Array->Montages[RandIndex];
		}
	}
	return nullptr;
}

void UAWeekCharacterAnimInstance::ChangeAnimOverride(FName State)
{
	mStatusKey = State;

	mSequenceMap = mAnimMap[mStatusKey].SequenceMap;
	mBlendSpaceMap = mAnimMap[mStatusKey].BlendSpaceMap;
	mMontageMap = mAnimMap[mStatusKey].MontageMap;
	mMontageArrayMap = mAnimMap[mStatusKey].MontageArrayMap;
}


