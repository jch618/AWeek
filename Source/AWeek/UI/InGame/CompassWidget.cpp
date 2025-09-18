// Fill out your copyright notice in the Description page of Project Settings.


#include "CompassWidget.h"

const TMap<int32, FString> UCompassWidget::DirectionMap = {
	{0, TEXT("N")},
	{45, TEXT("NE")},
	{90, TEXT("E")},
	{135, TEXT("SE")},
	{180, TEXT("S")},
	{225, TEXT("SW")},
	{270, TEXT("W")},
	{315, TEXT("NW")}
};

FString UCompassWidget::GetDirectionString(int32 Direction) const
{
	Direction %= 360;
	if (const FString* DirectionString = DirectionMap.Find(Direction))
	{
		return *DirectionString;
	}

	return FString::FromInt(Direction);
}
