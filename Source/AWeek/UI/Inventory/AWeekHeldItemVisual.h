// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekHeldItemVisual.generated.h"


class UAWeekItemBase;
class UTextBlock;
class UBorder;
class UImage;

UCLASS()
class AWEEK_API UAWeekHeldItemVisual : public UAWeekActivatableWidget
{
	GENERATED_BODY()
	
public:
	void InitializeHeldItemVisual(const TObjectPtr<UAWeekItemBase> InItem) const;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
};
