// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "PreviewObjectWidget.generated.h"

/**
 * 
 */
class UGridPlacedSubsystem;
UCLASS()
class AWEEK_API UPreviewObjectWidget : public UAWeekActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

	void SetupBuilding();
	void Cancel();
	void RotateR();
	void RotateL();

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* AttackMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AttackContextPriority = 0;

	UGridPlacedSubsystem* GridPlacedSubsystem = nullptr;

private:
	FUIActionBindingHandle LeftClickBindingHandle;
	FUIActionBindingHandle RightClickBindingHandle;
	FUIActionBindingHandle WheelUpBindingHandle;
	FUIActionBindingHandle WheelDownBindingHandle;
	
};
