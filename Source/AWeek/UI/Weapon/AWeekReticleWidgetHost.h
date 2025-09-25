// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekReticleDefinition.h"
#include "CommonUserWidget.h"
#include "UObject/NoExportTypes.h"
#include "AWeekReticleWidgetHost.generated.h"

class UAWeekReticleWidget;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekReticleWidgetHost : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ChangeWeapon(UAWeekReticleDefinition* ReticleDefinition);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnChangedWeapon(UAWeekReticleDefinition* ReticleDefinition);
};
