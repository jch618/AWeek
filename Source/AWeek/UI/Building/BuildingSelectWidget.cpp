// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Building/BuildingSelectWidget.h"

#include "BuildingWheelPanel.h"
#include "CommonLazyImage.h"
#include "NiagaraTypes.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Input/CommonUIInputTypes.h"


/*void UBuildingSelectWidget::DeactivateWidget()
{
	//TODO First Widget;
	
	Super::DeactivateWidget();
}*/

void UBuildingSelectWidget::NativeOnDeactivated()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	Super::NativeOnDeactivated();
	
}

void UBuildingSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	const FVector2D vpUMG  = UWidgetLayoutLibrary::GetViewportSize(this);   
	const float     dpi    = UWidgetLayoutLibrary::GetViewportScale(this);  
	CenterPosition  = (vpUMG / dpi) * 0.5f;
	
	if (ensure(WidgetSwitcher))
	{
		ChangeWheel(0);
		
	}
	
	FBindUIActionArgs ArgsL(FUIActionTag::TryConvert(FGameplayTag::RequestGameplayTag(TEXT("UI.Action.LeftClick"))),
		false,
		FSimpleDelegate::CreateUObject(this, &UBuildingSelectWidget::ActiveButton));
	LeftClickBindingHandle = RegisterUIActionBinding(ArgsL);

	FBindUIActionArgs ArgsR(FUIActionTag::TryConvert(FGameplayTag::RequestGameplayTag(TEXT("UI.Action.RightClick"))),
		false,
		FSimpleDelegate::CreateUObject(this, &UBuildingSelectWidget::ReturnButton));
	RightClickBindingHandle = RegisterUIActionBinding(ArgsR);
	//TEST Material Parameter
	/*WheelMID = BaseBuildingSelectWheel->GetDynamicMaterial();
	if (WheelMID)
	{
		UE_LOG(LogTemp, Log, TEXT("Wheel Material Instance TEst"));
		//ActiveRadius
		//ActiveRotation -> 마우스 위치 실시간
		//BodyOpacity
		//InnerRadius
		//OuterRadius
		//SectionCount ->	칸 개수
		//SelectedOpacity
		//SelectedRotation->선택된 칸 위치
	}*/
}

void UBuildingSelectWidget::NativeTick(const FGeometry& MyGeometry, float InDT)
{
	Super::NativeTick(MyGeometry, InDT);
	float SelectAngle = AngleDegFromSceenCenter(MyGeometry);
	//UE_LOG(LogTemp, Log, TEXT("scalar %.1f"), SelectAngle);
	

	if (SelectBuildingWheelPanel)
	{
		SelectBuildingWheelPanel->TickCount(SelectAngle);
	}
	
	/*float SelectPositionAngle = FindPosition(SelectAngle);
	UE_LOG(LogTemp, Log, TEXT("SelectPositionAngle %.1f"), SelectPositionAngle);
	if (WheelMID)
	{
		WheelMID->SetScalarParameterValue(TEXT("ActiveRotation"),SelectAngle);
		WheelMID->SetScalarParameterValue(TEXT("SelectedRotation"), SelectPositionAngle);
	}*/
}

void UBuildingSelectWidget::ChangeWheel(int index)
{
	//TODO Change Material Instance
	UE_LOG(LogTemp, Log, TEXT("index is %d"), index);
	WidgetSwitcher->SetActiveWidgetIndex(index);
	/*UWidget* Active = WidgetSwitcher->GetActiveWidget();
	SelectBuildingWheelPanel = Cast<UBuildingWheelPanel>(Active);*/
	
	UWidget* Active = WidgetSwitcher->GetWidgetAtIndex(index);
	if (!Active)
	{
		UE_LOG(LogTemp, Error, TEXT("Active widget is null"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Active widget: %s (%s)"),
		*Active->GetName(),
		*Active->GetClass()->GetName());

	UBuildingWheelPanel* NewPanel = Cast<UBuildingWheelPanel>(Active);
	if (!NewPanel)
	{
		UE_LOG(LogTemp, Error, TEXT("Active widget is NOT a UBuildingWheelPanel"));
		SelectBuildingWheelPanel = nullptr;
		return;
	}

	SelectBuildingWheelPanel = NewPanel;
	if (SelectBuildingWheelPanel)
	{
		UE_LOG(LogTemp, Log, TEXT("Change Wheel"));
		UE_LOG(LogTemp, Log,TEXT("Change wheel %s"), *SelectBuildingWheelPanel->GetFName().ToString());
		SelectBuildingWheelPanel->SetBuildingSelectWidget(this); SelectBuildingWheelPanel->ActivePanel();
	}

	/*if (WheelMID)
	{
		WheelMID->GetScalarParameterValue(TEXT("SectionCount"), SectionCount);
	}*/
	
}

float UBuildingSelectWidget::AngleDegFromSceenCenter(const FGeometry& G)
{
	const FVector2D vpSize = UWidgetLayoutLibrary::GetViewportSize(this);
	if (vpSize.IsNearlyZero()) return 0.f;
	
	
	const FVector2D mouseVP = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	

	
	const FVector2D v = mouseVP - CenterPosition;

	
	if (v.IsNearlyZero()) return 0.f;

	float angleDeg = FMath::RadiansToDegrees(FMath::Atan2(v.Y, v.X));
	
	angleDeg = FMath::Fmod(angleDeg + 360.f, 360.f);






	//LOG
	//
	//
	/*const FGeometry VpGeo = UWidgetLayoutLibrary::GetViewportWidgetGeometry(this);
	const FVector2D VpCenterAbs = VpGeo.GetAbsolutePosition() + VpGeo.GetAbsoluteSize() * 0.5f;

	UE_LOG(LogTemp, Log, TEXT("Viewport Center (Abs): %.1f, %.1f"), VpCenterAbs.X, VpCenterAbs.Y);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
			FString::Printf(TEXT("Center(abs): %.1f, %.1f"), VpCenterAbs.X, VpCenterAbs.Y));*/

	/*const FVector2D VpSize = UWidgetLayoutLibrary::GetViewportSize(this);
	UE_LOG(LogTemp, Log, TEXT("Viewport Size(px): %.0f x %.0f"), VpSize.X, VpSize.Y);*/

	/*const FVector2D vpUMG  = UWidgetLayoutLibrary::GetViewportSize(this);   // 1280x722
	const float     dpi    = UWidgetLayoutLibrary::GetViewportScale(this);  // 예: 1.5
	const FVector2D vpRaw  = vpUMG / dpi;                                   // ≈ 1920 x 1083

	UE_LOG(LogTemp, Log, TEXT("UMG: %.0f x %.0f  DPI: %.2f  Raw≈ %.0f x %.0f"),
		   vpUMG.X, vpUMG.Y, dpi, vpRaw.X, vpRaw.Y);*/

	/*const FVector2D MouseVP = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	UE_LOG(LogTemp, Log, TEXT("Mouse VP(px): %.1f, %.1f"), MouseVP.X, MouseVP.Y);*/
	
	return angleDeg * -1;
}

/*float UBuildingSelectWidget::FindPosition(float Angle)
{
	float sector = 360.f/SectionCount;
	int32 a = (Angle- (sector / 2))/sector;
	int32 A = Angle/(360/SectionCount);
	float I = 360.f / SectionCount;
	return (a*sector);
}*/

void UBuildingSelectWidget::ActiveButton()
{
	if (SelectBuildingWheelPanel)
	{
		SelectBuildingWheelPanel->ActiveWheel();
	}
}
void UBuildingSelectWidget::ReturnButton()
{
	if (SelectBuildingWheelPanel)
	{
		
	}
}





