// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "GameFramework/Actor.h"
#include "AWeek/UI/MainWidget//MyAWeekActivatableWidget.h"
#include "AWeek/UI/MainWidget/MainUIWidget.h"
#include "TestWidget.generated.h"

UCLASS(Blueprintable)
class AWEEK_API ATestWidget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestWidget();

	UPROPERTY(EditAnywhere, Category="Test")
	TSubclassOf<UMainUIWidget> MainWidgetClass;

	UPROPERTY(EditAnywhere, Category="Test")
	TObjectPtr<UMainUIWidget> MainWidget;

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TestStart();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
