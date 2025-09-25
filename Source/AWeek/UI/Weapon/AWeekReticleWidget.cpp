// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekReticleWidget.h"

void UAWeekReticleWidget::Init(UAWeekWeaponComponent* InWeaponComponent)
{
	WeaponComponentInstance = InWeaponComponent;
	OnInit();
}
