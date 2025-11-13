#pragma once
#include "CoreMinimal.h"
#include "DamageHitInfo.h"
#include "DamageInfo.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
    Default  UMETA(DisplayName = "Default"),
    Melee     UMETA(DisplayName = "Melee"),
    Projectile      UMETA(DisplayName = "Projectile"),
    Explosion   UMETA(DisplayName = "Explosion")
};

//How the Actor Should Response to the Damage
UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
    None UMETA(DisplayName = "None"),
    HitReaction     UMETA(DisplayName = "HitReaction"),
    Stagger      UMETA(DisplayName = "Stagger"),
    KnockBack   UMETA(DisplayName = "KnockBack")
};

USTRUCT(BlueprintType)
struct FDamageInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    float Amount = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    EDamageType DamageType = EDamageType::Default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    EDamageResponse DamageResponse = EDamageResponse::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    bool ShouldDamageInvincible = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    bool CanBeBlocked = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    bool CanBeParried = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    bool ShouldForceInterrupt = false; //Damage Force Interrupt Animation

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    TObjectPtr<AActor> DamageSource = nullptr; //Damage Instigator

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage System")
    FDamageHitInfo HitInfo;

};

