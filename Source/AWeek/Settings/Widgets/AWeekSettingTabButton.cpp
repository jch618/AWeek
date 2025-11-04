// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSettingTabButton.h"

void UAWeekSettingTabButton::SetText(const FText& InText)
{
	ButtonText = InText;
	OnTextUpdated(ButtonText);
}
