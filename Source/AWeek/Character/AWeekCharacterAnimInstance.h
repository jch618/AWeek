// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include"../Data/AWeekCharacterAnimInfo.h"
#include "AWeekCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* mAnimInfoDT;
protected:
	UPROPERTY(EditAnywhere)
	FName mStatusKey = TEXT("Default");

public : 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, FAWeekCharacterAnimInfo> mAnimMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimSequence>>	mSequenceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UBlendSpace>>	mBlendSpaceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimMontage>>	mMontageMap;

public:
	virtual void NativeBeginPlay();
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
public:
	UFUNCTION(BlueprintCallable)
	void InitializeAnimMap();

	UFUNCTION(BlueprintCallable)
	UAnimSequence* FindAnimSequence(const FName& Name);

	UFUNCTION(BlueprintCallable)
	UBlendSpace* FindBlendSpace(const FName& Name);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* FindAnimMontage(const FName& Name);

public: 
	void ChangeAnimOverride(FName State);
	FName GetCurrentOverride() { return mStatusKey; }
public:
	template<typename T>
	void SetAnimMontage_Interrupted_Delegate(UAnimMontage* Montage, T* Object, void(T::* Func)(UAnimMontage*, bool));
	template<typename T>
	void SetAnimMontage_End_Delegate(UAnimMontage* Montage, T* Object, void(T::* Func)(UAnimMontage*, bool));
};


template<typename T>
inline void UAWeekCharacterAnimInstance::SetAnimMontage_Interrupted_Delegate(UAnimMontage* Montage, T* Object, void(T::* Func)(UAnimMontage*, bool))
{
	if (!Montage || !Object) return;
	FOnMontageBlendingOutStarted BlendDel;
	BlendDel.BindUObject(Object, Func);
	Montage_SetBlendingOutDelegate(BlendDel, Montage);
}

template<typename T>
inline void UAWeekCharacterAnimInstance::SetAnimMontage_End_Delegate(UAnimMontage* Montage, T* Object, void(T::* Func)(UAnimMontage*, bool))
{
	if (!Montage || !Object) return;
	FOnMontageEnded EndDel;
	EndDel.BindUObject(Object, Func);
	Montage_SetEndDelegate(EndDel, Montage);
}
