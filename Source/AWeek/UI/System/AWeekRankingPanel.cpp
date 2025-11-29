// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekRankingPanel.h"
#include "Components/ListView.h"
#include "Kismet/GameplayStatics.h"
#include "AWeekRankingEntryWidget.h"
#include "RankEntryObject.h"

void UAWeekRankingPanel::RefreshRanking()
{
    if (!RankingListView)
    {
        UE_LOG(LogTemp, Warning, TEXT("RankingListView is null!"));
        return;
    }

    RankingListView->ClearListItems();

    UScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UScoreSubsystem>();
    if (!ScoreSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("ScoreSubsystem is null!"));
        return;
    }

    TArray<FRankEntry> RankList = ScoreSubsystem->LoadRankList();

    for (const FRankEntry& Entry : RankList)
    {
        URankEntryObject* Obj = URankEntryObject::CreateFrom(this, Entry);
        RankingListView->AddItem(Obj);
    }

    UE_LOG(LogTemp, Log, TEXT("RankingPanel refreshed. %d entries loaded."), RankList.Num());
}
