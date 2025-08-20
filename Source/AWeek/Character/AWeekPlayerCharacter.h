// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AWeekCharacter.h"
#include "../Player/AWeekPlayerAnimInstance.h"
#include "AWeekPlayerCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AWeekPlayerCharacter, Warning, All);

UCLASS()
class AWEEK_API AAWeekPlayerCharacter : public AAWeekCharacter
{
	GENERATED_BODY()
public:
	AAWeekPlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	TObjectPtr<UAWeekPlayerAnimInstance> mAnimInst;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSprint = false;

	UPROPERTY(EditAnywhere)
	float mWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	float mSprintSpeed = 600.f;

	UPROPERTY(EditAnywhere)
	float mRunToStopTime = 0.75f;

	UPROPERTY(EditAnywhere)
	float mSprintMinimumStamina = 25.f;

	UPROPERTY(VisibleAnywhere)
	float mSprintTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mSprintStaminaUsage = 25;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Attack(const FInputActionValue& Value);
	void SprintStart(const FInputActionValue& Value);
	void SprintCompleted();

public:
	UFUNCTION(BlueprintCallable)
	void FootStepEffect(FName SocketName);

	UFUNCTION()
	void AnimNotify_FootStepL()
	{
		FootStepEffect(TEXT("foot_l_Socket"));
	}

	UFUNCTION()
	void AnimNotify_FootStepR()
	{
		FootStepEffect(TEXT("foot_r_Socket"));
	}
	
};
