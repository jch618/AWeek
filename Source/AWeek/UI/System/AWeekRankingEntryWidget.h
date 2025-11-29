// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "../../System/Score/RankSaveData.h"
#include "AWeekRankingEntryWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekRankingEntryWidget : public UCommonUserWidget , public IUserObjectListEntry
{
	GENERATED_BODY()

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PlayerNameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ScoreText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> KillText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DayText;

public:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> RankText;
};
