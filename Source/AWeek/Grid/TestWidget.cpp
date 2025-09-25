// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TestWidget.h"

#include "AWeek/Player/AWeekUIController.h"
#include "Kismet/GameplayStatics.h"
#include "CommonUIExtensions.h"

// Sets default values
ATestWidget::ATestWidget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestWidget::BeginPlay()
{
	Super::BeginPlay();
	TestStart();
}

// Called every frame
void ATestWidget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestWidget::TestStart()
{
	UE_LOG(LogTemp, Warning, TEXT("UI Test Start!!!"));

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAWeekUIController* UIPC = Cast<AAWeekUIController>(PC);

	if (!IsValid(MainWidget) || !(MainWidget->IsActivated()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UI TestActicatableWidget is false!!"));
		if (MainWidgetClass)
		{
			//TestActiveatableWidget = Cast<
			MainWidget = Cast<UMainUIWidget, UCommonActivatableWidget>
			(UCommonUIExtensions::PushContentToLayer_ForPlayer(UIPC->GetLocalPlayer(),
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), MainWidgetClass))
				;
		}else{UE_LOG(LogTemp, Warning, TEXT("UI Test Failed!!!"));}

		UIPC->SetShowMouseCursor(true);
	}
	
}


