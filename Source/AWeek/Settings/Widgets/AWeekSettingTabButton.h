// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "AWeekSettingTabButton.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekSettingTabButton : public UCommonButtonBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = UI)
	void SetText(const FText& InText);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnTextUpdated(const FText& InText);

private:
	UPROPERTY(EditAnywhere)
	FText ButtonText;
};
