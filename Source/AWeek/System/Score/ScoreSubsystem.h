#pragma once

#include "CoreMinimal.h"
#include "GameEventMessageSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RankSaveData.h"
#include "../../System/AWeekEventMessageInfo.h"
#include "ScoreSubsystem.generated.h"

UENUM(BlueprintType)
enum class EScoreField : uint8
{
    Score           UMETA(DisplayName = "Score"),
    ZombieKillCount UMETA(DisplayName = "Zombie Kill Count"),
    Day             UMETA(DisplayName = "Day")
};

USTRUCT(BlueprintType)
struct FCurrentScoreEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 Score = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 ZombieKillCount = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 Day = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32, NewScore);

UCLASS()
class AWEEK_API UScoreSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UPROPERTY(BlueprintReadOnly)
    FCurrentScoreEntry CurrentEntry;

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount, EScoreField Field);

    UFUNCTION(BlueprintCallable)
    void ResetCurrentScore();

    UFUNCTION(BlueprintCallable)
    void SaveRankEntry(const FString& Name);

    UFUNCTION(BlueprintCallable)
    TArray<FRankEntry> LoadRankList();

    UPROPERTY(BlueprintAssignable)
    FOnScoreChanged OnScoreChanged;

private:
    FGameEventMessageListenerHandle DayChangedListenerHandle;

    void OnDayChanged(const FDayChangedMessage& Msg);
};
