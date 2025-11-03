// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSettingMain.h"

#include "AWeekSettingRegistry.h"

USettingRegistry* UAWeekSettingMain::CreateRegistry()
{
	UAWeekSettingRegistry* AWeekRegistry = NewObject<UAWeekSettingRegistry>();
	return AWeekRegistry;
}
