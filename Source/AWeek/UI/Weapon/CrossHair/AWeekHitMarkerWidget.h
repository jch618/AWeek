// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AWeek/System/GameEventMessageSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "AWeekHitMarkerWidget.generated.h"

class SAWeekHitMarkerWidget;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekHitMarkerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	UFUNCTION()
	void HandleHitMessage();

	UPROPERTY(EditAnywhere)
	float HitDuration = 0.4f;
	
	UPROPERTY(EditAnywhere)
	TArray<FSlateBrush> HitImages;

	UPROPERTY(BlueprintReadOnly)
	float HitOpacity = 0.0f;
	
	TSharedPtr<SAWeekHitMarkerWidget> HitMarkerWidget;

	FGameEventMessageListenerHandle HitHandle; 
};
