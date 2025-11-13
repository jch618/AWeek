// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"


#include "BuildingWheelPanel.generated.h"


/**
 * 
 */
class UCommonLazyImage;
class UBuildingSelectWidget;
UCLASS()
class AWEEK_API UBuildingWheelPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDT) override;

	void SetSelectPosition(float Angle);

	void TickCount(float Angle);

	virtual void ActiveWheel();
	virtual void ReturnWheel();
	
	virtual void ActivePanel();
	virtual void DeactivatedPanel();

	void SetBuildingSelectWidget(UBuildingSelectWidget* Widget);
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnDeactivated() override;

	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* BuildingSelectWheel;

	UMaterialInstanceDynamic* WheelMID;

	UBuildingSelectWidget* BuildingSelectWidget;

	int32 index = 0;
	
	UPROPERTY(EditAnywhere)
	float SectionCount = 8.f;
	FVector2D CenterPosition;
	
	float FindPosition(float Angle);
	

private:
	FUIActionBindingHandle LeftClickBindingHandle;
};
