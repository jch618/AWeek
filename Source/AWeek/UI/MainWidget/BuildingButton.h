// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "GameplayTagContainer.h"
#include "AWeek/Data/AWeekBuildingData.h"
#include "AWeek/Grid/PreviewObject.h"
#include "BuildingButton.generated.h"

/**
 * 
 */
class UCommonTextBlock;
class UCommonLazyImage;
class UBuildingCraftPanel;

UCLASS()
class AWEEK_API UBuildingButton : public UCommonButtonBase
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;


public:
	void SetImage(UTexture2D* Image);
	void SetName(const FText& Name);

	UPROPERTY()
	bool bActive = false;

	UPROPERTY()
	UBuildingCraftPanel* Panel = nullptr;

	UPROPERTY(EditAnywhere, Category="Panel")
	TSubclassOf<UBuildingCraftPanel> PanelClass;

	UBuildingCraftPanel* CreateOrGetPanel(const FAWeekBuildingData* RowData);

	//UFUNCTION(BlueprintCallable, Category="CloseLayer")
	void HandleClicked(const FAWeekBuildingData* Data);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CloseLayer")
	FGameplayTag LayerTag = FGameplayTag::RequestGameplayTag(TEXT("UI.Layer.GameMenu"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CloseLayer")
	bool bPopAll = true;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gird")
	TSubclassOf<APreviewObject> PreviewObjectClass = nullptr;


	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* BuildingName;
	UPROPERTY(meta=(BindWidget))
	UCommonLazyImage* BuildingIcon;
	
	
	

	
};
