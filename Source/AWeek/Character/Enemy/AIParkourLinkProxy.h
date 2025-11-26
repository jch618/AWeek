// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "GameplayTagContainer.h"
#include "AIParkourLinkProxy.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API AAIParkourLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()
public:
    AAIParkourLinkProxy();
protected : 
	virtual void BeginPlay() override;
    UFUNCTION()
    void HandleSmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint);

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vault")
    FGameplayTag RequiredTag;

};
