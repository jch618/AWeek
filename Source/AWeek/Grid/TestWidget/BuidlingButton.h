// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "GameplayTagContainer.h"
#include "AWeek/Grid/PreviewObject.h"
#include "BuidlingButton.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UBuidlingButton : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CloseLayer")
	FGameplayTag LayerTag = FGameplayTag::RequestGameplayTag(TEXT("UI.Layer.GameMenu"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CloseLayer")
	bool bPopAll = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gird")
	TSubclassOf<APreviewObject> PreviewObjectClass = nullptr;

	
	

	UFUNCTION(BlueprintCallable, Category="CloseLayer")
	void HandleClicked();

	
	
};
