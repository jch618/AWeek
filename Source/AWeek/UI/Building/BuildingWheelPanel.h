// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"


#include "BuildingWheelPanel.generated.h"


/**
 * 
 */
class UCommonActivatableWidgetSwitcher;
class UCommonLazyImage;
class UBuildingSelectWidget;
struct FAWeekBuildingData;
class UCanvasPanel;
class APreviewObject;
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

	void RemoveItem();

	void SetBuildingSelectWidget(UBuildingSelectWidget* Widget);
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnDeactivated() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	UPROPERTY(meta=(BindWidget))
	UCommonActivatableWidgetSwitcher* NotionSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Images")
	TArray<TObjectPtr<UCommonLazyImage>> Images;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	TArray<FDataTableRowHandle> BuildingRows;

	
	
	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* BuildingSelectWheel;

	UMaterialInstanceDynamic* WheelMID;

	UBuildingSelectWidget* BuildingSelectWidget;

	int32 Index = 0;
	int32 PreIndex = 0;
	
	UPROPERTY(EditAnywhere)
	float SectionCount = 8.f;
	FVector2D CenterPosition;
	
	float FindPosition(float Angle);
	
	bool CheckItem = true;
	void UpdateData(int32 I);

	UPROPERTY()
	TSubclassOf<APreviewObject> PreviewObjectClass;
private:
	FUIActionBindingHandle LeftClickBindingHandle;
};
