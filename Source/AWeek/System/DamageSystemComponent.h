// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageInfo.h"
#include "IDamageAble.h"
#include "DamageSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlocked, bool, IsParried);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageResponse, EDamageResponse, DamageResponse);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UDamageSystemComponent : public UActorComponent , public IDamageAble
{
	GENERATED_BODY()
public:
    UDamageSystemComponent();
protected :
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float MaxHealth = 100.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
    float Health = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    bool IsInvincible = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
    bool IsDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    bool IsInterruptable = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    bool IsBlocking = false;

public:
    virtual float GetCurrentHealth_Implementation() const override;
    virtual float GetMaxHealth_Implementation() const override;
    virtual float Heal_Implementation(float Amount) override;
    virtual bool TakeDamage_Implementation(FDamageInfo DamageInfo) override;

protected:
	virtual void BeginPlay() override;
public:
    UPROPERTY(BlueprintAssignable, Category = "Damage")
    FOnDeath OnDeath;
    UPROPERTY(BlueprintAssignable, Category = "Damage")
    FOnBlocked OnBlocked;
    UPROPERTY(BlueprintAssignable, Category = "Damage")
    FOnDamageResponse OnDamageResponse;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


};
