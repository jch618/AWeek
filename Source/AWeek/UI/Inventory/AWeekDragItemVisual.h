// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AWeek/UI/AWeekActivatableWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AWeekDragItemVisual.generated.h"


class UTextBlock;
class UBorder;
class UImage;

UCLASS()
class AWEEK_API UAWeekDragItemVisual : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;
};
