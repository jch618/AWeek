// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "UObject/NoExportTypes.h"
#include "AWeekGameInput.generated.h"

UCLASS()
class AWEEK_API UAWeekGameInput : public UObject
{
	GENERATED_BODY()

public:
	UAWeekGameInput();

public:
	TObjectPtr<UInputMappingContext>	mContext;
	TObjectPtr<UInputAction>			mMove;
	TObjectPtr<UInputAction>			mLook;
	TObjectPtr<UInputAction>			mJump;
	TObjectPtr<UInputAction>			mSprint;
	TObjectPtr<UInputAction>			mAttack;
	TObjectPtr<UInputAction>			mInventory;
	
};
