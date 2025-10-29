// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "AWeek/UI/MainWidget/TestPanelToButton.h"

#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "ButtonPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UButtonPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* Button_0;
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UOverlay* FloatingLayer;
	/*UPROPERTY(EditAnywhere,meta=(BindWidget))
	USizeBox* SizeBox;*/

	//UPROPERTY(EditAnywhere, meta=(BindWidget))

	

	UPROPERTY(EditAnywhere, Category="Panel")
	TSubclassOf<UTestPanelToButton> TestPanelToButtonClass;

	UPROPERTY(EditAnywhere, Category="Panel")
	TObjectPtr<UTestPanelToButton> TestPanelToButton;
	

	UFUNCTION()
	void HandleToggleClicked();
	
	virtual void NativeConstruct() override;

	bool bPanelVisible = true;
	
	
};
