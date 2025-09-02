#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekInteractionWidget.generated.h"

class AAWeekPlayerCharacter;
struct FAWeekInteractableData;
class UTextBlock;
class UProgressBar;

UCLASS()
class AWEEK_API UAWeekInteractionWidget : public UAWeekActivatableWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	AAWeekPlayerCharacter* PlayerReference;

	void UpdateWidget(const FAWeekInteractableData* FInteractableData) const;

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
