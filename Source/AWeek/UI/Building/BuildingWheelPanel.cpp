// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Building/BuildingWheelPanel.h"

#include "BuildingSelectNotionPanel.h"
#include "Components/CanvasPanel.h"
#include "AWeek/Data/AWeekBuildingData.h"
#include "AWeek/UI/Building/BuildingSelectWidget.h"
#include "CommonLazyImage.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Grid/GridPlacedSubsystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AWeek/UI/Building/BuildingNotionPanel.h"

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

	//Find BuildingIcon Image
	Images.Reset();
	if (CanvasPanel)
	{
		const int32 ChildCount = CanvasPanel->GetChildrenCount();
		for (int32 i = 0; i < ChildCount; i++)
		{
			if (UWidget* Child = CanvasPanel->GetChildAt(i))
			{
				if (UCommonLazyImage* Lazy = Cast<UCommonLazyImage>(Child))
				{
					const FString widgetName = Child->GetName();
					if (widgetName == TEXT("BuildingSelectWheel")){continue;}
					Images.Add(Lazy);
				}
			}
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("Images size %d"), Images.Num());

	//SetImage
	const int32 Count = FMath::Min(Images.Num(), BuildingRows.Num());
	for (int32 i = 0; i< Count; i++)
	{
		UCommonLazyImage* ImageWidget = Images[i];
		if (!ImageWidget)
		{
			continue;
		}

		const FDataTableRowHandle& Handle = BuildingRows[i];
		
		
		const FAWeekBuildingData* Row = Handle.GetRow<FAWeekBuildingData>(TEXT("MyBuildingWidget"));
		if (!Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Building Row at index %d"), i);
			continue;
		}
		if (Row->Image)
		{
			ImageWidget->SetBrushFromTexture(Row->Image);
			//ImageWidget->SetBrushFromMaterial(Row->IconMaterial);
		}
		
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
	Index *= -1;
	if (Index == SectionCount)Index = 0;
	if (Index != PreIndex)
	{
		PreIndex = Index;
		UpdateData(Index);
		NotionSwitcher->SetActiveWidgetIndex(Index);
		UWidget* Active = NotionSwitcher->GetWidgetAtIndex(Index);
		if (!Active)
		{
			UE_LOG(LogTemp, Error, TEXT("Active widget is null"));
			return;
		}
		UBuildingNotionPanel* BuildingNotionPanel = Cast<UBuildingNotionPanel>(Active);
		if (BuildingNotionPanel)
		{
			if (!BuildingRows.IsValidIndex(Index))
			{
				UE_LOG(LogTemp, Warning, TEXT("BuildingRows invalid index: %d"), Index);
				//PreviewObjectClass = nullptr;
				return;
			}
			const FDataTableRowHandle& Handle = BuildingRows[Index];
			//NULL Check
			if (Handle.DataTable == nullptr || Handle.RowName.IsNone())
			{
				UE_LOG(LogTemp, Warning, TEXT("BuildingRows[%d] has no valid DataTable or RowName"), Index);
				//PreviewObjectClass = nullptr;
				return;
			}
		
			const FAWeekBuildingData* Row = Handle.GetRow<FAWeekBuildingData>(TEXT("MyBuildingWidget"));
			
			BuildingNotionPanel->UpdateData(Row);
		}
	}
}



float UBuildingWheelPanel::FindPosition(float Angle)
{
	float sector = 360.f/SectionCount;
	Index = (Angle- (sector / 2))/sector;
	if (Index == -360) Index = 0;
	/*int32 A = Angle/(360/SectionCount);
	float I = 360.f / SectionCount;*/
	return (Index*sector);
}

void UBuildingWheelPanel::ActiveWheel()
{
	
	UE_LOG(LogTemp, Log, TEXT("Building select widget click action %d"), Index);
	//Check FAWeekBuildingData in TArray
	/*if (!BuildingRows.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildingRows invalid index: %d"), Index);
		return;
	}
	const FDataTableRowHandle& Handle = BuildingRows[Index];
	//NULL Check
	if (Handle.DataTable == nullptr || Handle.RowName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildingRows[%d] has no valid DataTable or RowName"), Index);
		return;
	}
		
	const FAWeekBuildingData* Row = Handle.GetRow<FAWeekBuildingData>(TEXT("MyBuildingWidget"));*/
	UWidget* Widget = NotionSwitcher->GetWidgetAtIndex(Index);
	UBuildingNotionPanel* NotionPanel = Cast<UBuildingNotionPanel>(Widget);
	if (!NotionPanel->bTrue){return;}
	SelectNotionPanel = NotionPanel;
	if (CheckItem && PreviewObjectClass)
	{
		//TODO GridSystem
		UE_LOG(LogTemp, Log, TEXT("PreviewObjectClass TEST_ %s"), *PreviewObjectClass->GetName());

		UE_LOG(LogTemp, Log, TEXT("0"));
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!PC)return;

		if (UGridPlacedSubsystem* Sub = GetWorld()->GetSubsystem<UGridPlacedSubsystem>())
		{
			AAWeekPlayerCharacter* Ch = Cast<AAWeekPlayerCharacter>(PC->GetPawn());
			Ch->ToggleBuildingWidget();
			Sub->StartPlacement(PreviewObjectClass, PC, this);

		}
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("PreviewObjectClass is nullptr"));
	}
	
}
void UBuildingWheelPanel::UpdateData(int32 I)
{
	//TODO Find Building
	if (!BuildingRows.IsValidIndex(I))
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildingRows invalid index: %d"), I);
		PreviewObjectClass = nullptr;
		return;
	}
	const FDataTableRowHandle& Handle = BuildingRows[I];
	//NULL Check
	if (Handle.DataTable == nullptr || Handle.RowName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("BuildingRows[%d] has no valid DataTable or RowName"), I);
		PreviewObjectClass = nullptr;
		return;
	}
		
	const FAWeekBuildingData* Row = Handle.GetRow<FAWeekBuildingData>(TEXT("MyBuildingWidget"));
	PreviewObjectClass = Row->PreviewObjectClass.LoadSynchronous();
	
	//TODO Update CraftPanel
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
	NotionSwitcher->SetActiveWidgetIndex(0);
}


void UBuildingWheelPanel::RemoveItem()
{
	if (SelectNotionPanel)
	{
		if (UBuildingSelectNotionPanel* SelectPanel = Cast<UBuildingSelectNotionPanel>(SelectNotionPanel))
			SelectPanel->RemoveItem();
	}
}


