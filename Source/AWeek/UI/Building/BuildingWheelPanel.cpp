// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Building/BuildingWheelPanel.h"

#include "AWeek/UI/Building/BuildingSelectWidget.h"
#include "CommonLazyImage.h"
#include "Blueprint/WidgetLayoutLibrary.h"



void UBuildingWheelPanel::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (WheelMID)
	{
		WheelMID->SetScalarParameterValue(TEXT("SectionCount"),SectionCount);
	}else
	{
		WheelMID = BuildingSelectWheel->GetDynamicMaterial();
		/*if (!WheelMID){return;}
		WheelMID->SetScalarParameterValue(TEXT("SectionCount"),SectionCount);*/
	}

	
}

void UBuildingWheelPanel::NativeConstruct()
{
	Super::NativeConstruct();

	
}
void UBuildingWheelPanel::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}


void UBuildingWheelPanel::NativeTick(const FGeometry& MyGeometry, float InDT)
{
	Super::NativeTick(MyGeometry, InDT);


	//float SelectAngle = AngleDegFromSceenCenter();
	//UE_LOG(LogTemp, Log, TEXT("scalar %.1f"), SelectAngle);
	
}

void UBuildingWheelPanel::TickCount(float angle)
{
	float SelectPositionAngle = FindPosition(angle);
	//UE_LOG(LogTemp, Log, TEXT("SelectPositionAngle %.1f"), SelectPositionAngle);
	//UE_LOG(LogTemp, Log, TEXT("Widget name %s"), *GetFName().ToString());
	if (WheelMID)
	{
		WheelMID->SetScalarParameterValue(TEXT("ActiveRotation"),angle);
		WheelMID->SetScalarParameterValue(TEXT("SelectedRotation"), SelectPositionAngle);
	}else
	{
		//UE_LOG(LogTemp, Log, TEXT("2"));
	}
	index *= -1;
}



float UBuildingWheelPanel::FindPosition(float Angle)
{
	float sector = 360.f/SectionCount;
	index = (Angle- (sector / 2))/sector;
	if (index == -360) index = 0;
	/*int32 A = Angle/(360/SectionCount);
	float I = 360.f / SectionCount;*/
	return (index*sector);
}

void UBuildingWheelPanel::ActiveWheel()
{
	if (index == SectionCount)index = 0;
	UE_LOG(LogTemp, Log, TEXT("Building select widget click action %d"), index);
}
void UBuildingWheelPanel::ReturnWheel()
{
	if (BuildingSelectWidget)
	{
		BuildingSelectWidget->ChangeWheel(0);
	}
}

void UBuildingWheelPanel::SetBuildingSelectWidget(UBuildingSelectWidget* Widget)
{
	BuildingSelectWidget = Widget;
}

void UBuildingWheelPanel::ActivePanel()
{
	UE_LOG(LogTemp, Log, TEXT("Click TEST"));
	const FVector2D vpUMG  = UWidgetLayoutLibrary::GetViewportSize(this);   // 1280x722
	const float     dpi    = UWidgetLayoutLibrary::GetViewportScale(this);  // 예: 1.5
	CenterPosition  = (vpUMG / dpi) * 0.5f;

	
}

void UBuildingWheelPanel::DeactivatedPanel()
{
	
}

