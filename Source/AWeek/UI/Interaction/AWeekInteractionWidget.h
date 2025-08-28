// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekInteractionWidget.generated.h"

class AAWeekPlayerCharacter;
struct FInteractableData;
class UTextBlock;
class UProgressBar;

UCLASS()
class AWEEK_API UAWeekInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	AAWeekPlayerCharacter* PlayerReference;

	void UpdateWidget(const TObjectPtr<FInteractableData> InteractableData) const;

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UTextBlock> ActionText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UTextBlock> QuantityText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UTextBlock> KeyPressText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	TObjectPtr<UProgressBar> InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrentInteractionDuration;

	UFUNCTION(Category = "InteractionWidget | Interactable Data")
	float UpdateInteractionProgress();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
