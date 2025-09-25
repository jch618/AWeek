// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekCharacterAnimInfo.generated.h"

USTRUCT(BlueprintType)
struct FAnimMontageArray
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimMontage>> Montages;
};

USTRUCT(BlueprintType)
struct AWEEK_API FAWeekCharacterAnimInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UAnimSequence>>	SequenceMap;

	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UBlendSpace>>	BlendSpaceMap;

	UPROPERTY(EditAnywhere)
	TMap<FName, TObjectPtr<UAnimMontage>>	MontageMap;

	UPROPERTY(EditAnywhere)
	TMap<FName, FAnimMontageArray> MontageArrayMap;

};