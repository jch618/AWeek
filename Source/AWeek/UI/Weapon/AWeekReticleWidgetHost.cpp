// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekReticleWidgetHost.h"

void UAWeekReticleWidgetHost::ChangeWeapon(UAWeekReticleDefinition* ReticleDefinition)
{
	OnChangedWeapon(ReticleDefinition);
}
