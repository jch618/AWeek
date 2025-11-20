// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidgetSwitcher.h"

#include "AWeek/UI/AWeekActivatableWidget.h"
#include "BuildingSelectWidget.generated.h"


/**
 * 
 */
class UCommonLazyImage;
class UBuildingWheelPanel;

UCLASS()
class AWEEK_API UBuildingSelectWidget : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	//virtual void DeactivateWidget();

	virtual void NativeConstruct() override;
	virtual void NativeOnDeactivated() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDT) override;

	void ChangeWheel(int index);

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonActivatableWidgetSwitcher* WidgetSwitcher;
	/*UPROPERTY(meta=(BindWidget))
	UCommonActivatableWidgetSwitcher* NotionSwitcher;*/

	UPROPERTY(meta=(BindWidget))
	UBuildingWheelPanel* BaseBuildingWheelPanel;

	UBuildingWheelPanel* SelectBuildingWheelPanel;

	/*UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* BaseBuildingSelectWheel;*/

	UMaterialInterface* WheelMaterialInstance;
	UMaterialInstanceDynamic* WheelMID;

	
	float AngleDegFromSceenCenter(const FGeometry& Fg);

	float SectionCount = 3.f;

	//float FindPosition(float Angle);

	FVector2D CenterPosition;
	


	UFUNCTION()
	void ActiveButton();
	UFUNCTION()
	void ReturnButton();
	
private:
	FUIActionBindingHandle LeftClickBindingHandle;
	FUIActionBindingHandle RightClickBindingHandle;
};
