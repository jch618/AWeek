// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInfo.h"
#include "IDamageAble.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UDamageAble : public UInterface
{
	GENERATED_BODY()
};

class IDamageAble 
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	 float GetCurrentHealth() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	 float GetMaxHealth() const;
	 UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	 bool GetIsDead(); //return WasDamaged(bool) -> some characters doesn't get damage like invincible player
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	 float Heal(float amount); //return resultHealth(float) After healing
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	bool TakeDamage(FDamageInfo DamageInfo); //return WasDamaged(bool) -> some characters doesn't get damage like invincible player
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	bool ReserveAttackToken(int Amount); //Reserve Attack Token
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void ReturnAttackToken(int Amount); //Return Attack Token
};




