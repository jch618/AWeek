// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekRankingEntryWidget.h"
#include "Components/TextBlock.h"
#include"RankEntryObject.h"
#include"AWeekRankingPanel.h"
#include <Components/ListView.h>

void UAWeekRankingEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    URankEntryObject* RankObj = Cast<URankEntryObject>(ListItemObject);
    if (!RankObj) return;

    UScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UScoreSubsystem>();


    const FRankEntry& Entry = RankObj->Entry;

    if (PlayerNameText) PlayerNameText->SetText(FText::FromString(Entry.Name));
    if (ScoreText) ScoreText->SetText(FText::AsNumber(Entry.Score));
    if (KillText) KillText->SetText(FText::AsNumber(Entry.ZombieKillCount));
    if (DayText) DayText->SetText(FText::AsNumber(Entry.Day));

    if (RankText && ScoreSubsystem)
    {
        int32 Rank = ScoreSubsystem->GetRank(Entry.EntryID);
        RankText->SetText(FText::FromString(FString::Printf(TEXT("#%d"), Rank)));

        if (Entry.EntryID == ScoreSubsystem->CurrentRunID)
        {
            RankText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
            PlayerNameText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
            ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
            KillText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
            DayText->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));

        }
        else
        {
            RankText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
            PlayerNameText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
            ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
            KillText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
            DayText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
        }
    }
}