#include "ScoreSubsystem.h"
#include "Kismet/GameplayStatics.h"

#define RANK_SAVE_SLOT  TEXT("RankSlot")
#define SAVE_INDEX      0

void UScoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (UGameInstance* GI = GetGameInstance())
    {
        if (UGameEventMessageSubsystem* MsgSubsystem = GI->GetSubsystem<UGameEventMessageSubsystem>())
        {
            DayChangedListenerHandle = MsgSubsystem->RegisterListener<FDayChangedMessage>(
                FGameplayTag::RequestGameplayTag(FName("Event.DayChanged")),
                [this](FGameplayTag Channel, const FDayChangedMessage& Msg)
                {
                    this->OnDayChanged(Msg);
                }
            );
        }
    }
}

void UScoreSubsystem::Deinitialize()
{
    if (DayChangedListenerHandle.IsValid())
    {
        DayChangedListenerHandle.Unregister();
    }
    Super::Deinitialize();
}

void UScoreSubsystem::AddScore(int32 Amount, EScoreField Field)
{
    switch (Field)
    {
    case EScoreField::Score:
        CurrentEntry.Score += Amount;
        break;

    case EScoreField::ZombieKillCount:
        CurrentEntry.ZombieKillCount += Amount;
        break;

    case EScoreField::Day:
        CurrentEntry.Day += Amount;
        break;

    default:
        break;
    }
    OnScoreChanged.Broadcast(CurrentEntry.Score);
}

void UScoreSubsystem::ResetCurrentScore()
{
    CurrentEntry = FCurrentScoreEntry{};
}

//Save Directory : C:\UEProject\AWeek\Saved\SaveGames

void UScoreSubsystem::SaveRankEntry(const FString& Name)
{
    URankSaveData* Data = nullptr;

    if (UGameplayStatics::DoesSaveGameExist(RANK_SAVE_SLOT, SAVE_INDEX))
        Data = Cast<URankSaveData>(UGameplayStatics::LoadGameFromSlot(RANK_SAVE_SLOT, SAVE_INDEX));
    else
        Data = Cast<URankSaveData>(UGameplayStatics::CreateSaveGameObject(URankSaveData::StaticClass()));

    FRankEntry NewEntry;
    NewEntry.Name = Name;
    NewEntry.Score = CurrentEntry.Score;
    NewEntry.ZombieKillCount = CurrentEntry.ZombieKillCount;
    NewEntry.Day = CurrentEntry.Day;
    NewEntry.EntryID = CurrentRunID;

    Data->RankList.Add(NewEntry);

    Data->RankList.Sort([](const FRankEntry& A, const FRankEntry& B)
        {
            return A.Score > B.Score;
        });

    UGameplayStatics::SaveGameToSlot(Data, RANK_SAVE_SLOT, SAVE_INDEX);

    CurrentEntry = FCurrentScoreEntry{};

}

TArray<FRankEntry> UScoreSubsystem::LoadRankList()
{
    TArray<FRankEntry> RankList;

    if (UGameplayStatics::DoesSaveGameExist(RANK_SAVE_SLOT, SAVE_INDEX))
    {
        URankSaveData* Data = Cast<URankSaveData>(
            UGameplayStatics::LoadGameFromSlot(RANK_SAVE_SLOT, SAVE_INDEX)
        );

        if (Data)
        {
            RankList = Data->RankList;

            RankList.Sort([](const FRankEntry& A, const FRankEntry& B)
                {
                    return A.Score > B.Score;
                });

            EntryRankMap.Empty();
            int32 CurrentRank = 1;
            for (int32 i = 0; i < RankList.Num(); ++i)
            {
                const FRankEntry& Entry = RankList[i];

                if (i > 0 && Entry.Score == RankList[i - 1].Score)
                {
                    EntryRankMap.Add(Entry.EntryID, EntryRankMap[RankList[i - 1].EntryID]);
                }
                else
                {
                    EntryRankMap.Add(Entry.EntryID, CurrentRank);
                }

                CurrentRank++;
            }
        }
    }

    return RankList;
}


void UScoreSubsystem::OnDayChanged(const FDayChangedMessage& Msg)
{
    CurrentEntry.Day = Msg.Day;
    CurrentEntry.Score += 10000;
    OnScoreChanged.Broadcast(CurrentEntry.Score);

}

void UScoreSubsystem::StartNewRun()
{
    CurrentRunID = FGuid::NewGuid();// New Play Session ID
    CurrentEntry = FCurrentScoreEntry{};
}

int32 UScoreSubsystem::GetRank(const FGuid& EntryID) const
{
    if (EntryRankMap.Contains(EntryID))
    {
        return EntryRankMap[EntryID];
    }

    return 0;
}
