// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../UI/AWeekActivatableWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "AWeekRankingPanel.h"
#include"AWeekRankingMain.h"
#include "AWeekGameOverPanel.generated.h"


UCLASS()
class AWEEK_API UAWeekGameOverPanel : public UAWeekActivatableWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable)
    void InitializeGameOverPanel();

    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> PlayerNameTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ScoreText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> KillCountText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DayText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SubmitScoreButton;

    UPROPERTY(EditDefaultsOnly, Category = "Ranking")
    TSubclassOf<UAWeekRankingPanel> RankingPanelClass;

    UPROPERTY(EditDefaultsOnly, Category = "Ranking")
    TSubclassOf<UAWeekRankingMain> RankingMain;

private:
    UFUNCTION()
    void HandleSubmitScoreClicked();
    UFUNCTION()
    void HandleTextChanged(const FText& Text);

    int32 CurrentScore = 0;
    int32 CurrentKillCount = 0;
    int32 CurrentDay = 0;
	
};
