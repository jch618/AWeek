// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekRankingMain.h"
#include "AWeekRankingPanel.h"


void UAWeekRankingMain::NativeOnActivated()
{
    Super::NativeOnActivated();

    if (RankingPanel)
    {
        RankingPanel->RefreshRanking();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("RankingPanel is null in RankingMain"));
    }
}
