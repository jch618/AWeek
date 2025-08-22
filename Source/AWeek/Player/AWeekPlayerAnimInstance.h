// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Data/AWeekPlayerAnimInfo.h"
#include "Animation/AnimInstance.h"
#include "AWeekPlayerAnimInstance.generated.h"

UCLASS()
class AWEEK_API UAWeekPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	FName mStatusKey = TEXT("Default");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, FPlayerAnimInfo> mAnimMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimSequence>>	mSequenceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UBlendSpace>>	mBlendSpaceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimMontage>>	mMontageMap;

	UAnimMontage* mRunToStopMontage;

	UAnimMontage* mOneHandVaultMontage;

public:
	virtual void NativeBeginPlay();
	virtual void NativeInitializeAnimation();

	virtual void NativeUpdateAnimation(float DeltaSeconds);

protected:
	UFUNCTION(BlueprintCallable)
	UAnimSequence* FindAnimSequence(const FName& Name);

	UFUNCTION(BlueprintCallable)
	UBlendSpace* FindBlendSpace(const FName& Name);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* FindAnimMontage(const FName& Name);

public:
	void ChangeAnimOverride(FName State)
	{
		mStatusKey = State;

		mSequenceMap = mAnimMap[mStatusKey].SequenceMap;
		mBlendSpaceMap = mAnimMap[mStatusKey].BlendSpaceMap;
		mMontageMap = mAnimMap[mStatusKey].MontageMap;

		mRunToStopMontage = FindAnimMontage(TEXT("RunToStop"));
		mOneHandVaultMontage = FindAnimMontage(TEXT("OneHandVault"));
	}

	void PlayRunToStopMontage()
	{
		Montage_Play(mRunToStopMontage);
	}

	bool IsPlayingRunToStopMontage()
	{
		return Montage_IsPlaying(mRunToStopMontage);
	}

	void PlayVaultMontage()
	{
		Montage_Play(mOneHandVaultMontage);
	}

protected:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
