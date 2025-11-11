// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_RegisterInput.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AWEEK_API UGameFeatureAction_RegisterInput : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	virtual void OnGameFeatureRegistering() override;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AssetBundles="Client"))
	TArray<TObjectPtr<UInputMappingContext>> InputMappings;

private:
	void RegisterInputMappingContexts(ULocalPlayer* LocalPlayer);
};
