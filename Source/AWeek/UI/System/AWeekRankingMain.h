// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "AWeekRankingMain.generated.h"

class UAWeekRankingPanel;

UCLASS()
class AWEEK_API UAWeekRankingMain : public UCommonActivatableWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAWeekRankingPanel> RankingPanel;

	virtual void NativeOnActivated() override;
};
