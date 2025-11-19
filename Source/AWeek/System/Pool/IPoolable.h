// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"UObject/Interface.h"
#include "IPoolable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UPoolable : public UInterface
{
    GENERATED_BODY()
};

class IPoolable
{
    GENERATED_BODY()

public:
    //Read
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pooling")
    void OnLoadFromPool();
    //Write
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pooling")
    void OnStoreToPool();
};