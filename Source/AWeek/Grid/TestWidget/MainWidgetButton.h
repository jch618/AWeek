// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"

#include "CommonButtonBase.h"
#include "MainWidgetButton.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UMainWidgetButton : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* Text;

	UPROPERTY(meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button", meta=(ExposeOnSpawn = true))
	FText InitialText = FText::FromString(TEXT("Button"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Button", meta=(ExposeOnSpawn = true))
	TObjectPtr<UTexture2D> InitialIcon = nullptr;


	UFUNCTION(BlueprintCallable, Category="Button")
	void SetButtonText(const FText& InText);
	UFUNCTION(BlueprintCallable, Category="Button")
	void SetButtonIcon(UTexture2D* InIcon);

protected:
	virtual void NativePreConstruct() override;
};
