// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "AWeekReticleWidget.generated.h"

class UAWeekWeaponComponent;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekReticleWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(UAWeekWeaponComponent* InWeaponComponent);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnInit();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAWeekWeaponComponent> WeaponComponentInstance;
};
