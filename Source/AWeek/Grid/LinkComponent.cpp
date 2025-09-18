// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/LinkComponent.h"
#include "NavLinkCustomComponent.h"
#include "NavAreas/NavArea_Default.h"
#include "NavAreas/NavArea_Null.h"

// Sets default values for this component's properties
ULinkComponent::ULinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	NavLink = CreateDefaultSubobject<UNavLinkCustomComponent>("NavLink");

	//SetLinkData는 생성자에서 하지 말것
	/*NavLink->SetLinkData(FVector(0,0,0), FVector(0,200,0), ENavLinkDirection::BothWays);

	NavLink->SetEnabledArea(UNavArea_Default::StaticClass());
	NavLink->SetDisabledArea(UNavArea_Null::StaticClass());*/
	

	
	// ...
}

void ULinkComponent::OnRegister()
{
	Super::OnRegister();
	
}


// Called when the game starts
void ULinkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULinkComponent::SetPosition(FVector StartPosition, FVector EndPosition)
{
	NavLink->SetLinkData(StartPosition, EndPosition, ENavLinkDirection::BothWays);
}





