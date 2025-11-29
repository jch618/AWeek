// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../UI/AWeekActivatableWidget.h"
#include "../../System/Score/ScoreSubsystem.h"
#include "AWeekRankingPanel.generated.h"

class UListView;
class UAWeekRankingEntryWidget;

UCLASS()
class AWEEK_API UAWeekRankingPanel : public UCommonUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void RefreshRanking();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UListView> RankingListView;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UAWeekRankingEntryWidget> EntryWidgetClass;
};
