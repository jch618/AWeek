#include "EnemyActorSpawner.h"
#include "../Pool/ActorPoolSubSystem.h"
#include "../Pool/IPoolable.h"
#include"../Visibility/CameraVisibilitySubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

AEnemyActorSpawner::AEnemyActorSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEnemyActorSpawner::BeginPlay()
{
    UWorld* World = GetWorld();
    if (!World) return;

    if (UGameInstance* GI = World->GetGameInstance())
    {
        ActorPoolSubsystem = GI->GetSubsystem<UActorPoolSubSystem>();
        if (!ActorPoolSubsystem)
        {
            UE_LOG(LogTemp, Error, TEXT("[%s] Failed to get ActorPoolSubSystem!"), *GetName());
        }
    }

    VisibilitySubsystem = World->GetSubsystem<UCameraVisibilitySubsystem>();
    if (!VisibilitySubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("[%s] Failed to get CameraVisibilitySubsystem!"), *GetName());
    }

    auto& MessageSubsystem = UGameEventMessageSubsystem::Get(this);

    DayChangedListenerHandle = MessageSubsystem.RegisterListener<FDayChangedMessage>(
        FGameplayTag::RequestGameplayTag(FName("Event.DayChanged")),
        [this](FGameplayTag Channel, const FDayChangedMessage& Msg)
        {
            OnDayChanged(Msg);
        });
}

void AEnemyActorSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    DayChangedListenerHandle.Unregister();
    Super::EndPlay(EndPlayReason);
}

void AEnemyActorSpawner::OnDayChanged(const FDayChangedMessage& Msg)
{
    if (Msg.bIsDay)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] NightEnd!! Day: %d"), *GetName(), Msg.Day);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] Night Begin!! Day: %d"), *GetName(), Msg.Day);
        SpawnEnemiesFromPool();
    }
}

void AEnemyActorSpawner::SpawnEnemiesFromPool()
{
    if (!ActorPoolSubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] ActorPoolSubsystem is null"), *GetName());
        return;
    }

    if (!VisibilitySubsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] VisibilitySubsystem is null"), *GetName());
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    for (const FSpawnInfo& Info : SpawnList)
    {
        if (!Info.ActorClass) continue;

        int32 ActiveCount = SpawnedEnemies.Num();
        int32 AvailableSlots = FMath::Max(0, MaxActiveEnemies - ActiveCount);
        int32 SpawnNum = FMath::Min(Info.SpawnCount, AvailableSlots);

        for (int32 i = 0; i < SpawnNum; i++)
        {
            FVector SpawnLocation = GetRandomLocationInVolumes();
            if (SpawnLocation.IsZero()) continue;

            float ZOffset = 0.f;
            if (ACharacter* DefaultChar = Cast<ACharacter>(Info.ActorClass->GetDefaultObject()))
            {
                if (UCapsuleComponent* Capsule = DefaultChar->GetCapsuleComponent())
                    ZOffset = Capsule->GetScaledCapsuleHalfHeight();
            }

            SpawnLocation.Z += ZOffset;

            //bool bVisibleToPlayer = VisibilitySubsystem->IsLocationVisible(SpawnLocation, false);

            //if (bVisibleToPlayer)
            //{
            //    UE_LOG(LogTemp, Log, TEXT("[%s] Skip spawn: player can see location (%s)"),
            //        *GetName(), *SpawnLocation.ToString());
            //    continue;
            //}

            FRotator SpawnRot = bSpawnActorRandomRotation
                ? FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f)
                : FRotator::ZeroRotator;

            ABaseEnemy* SpawnedEnemy = Cast<ABaseEnemy>(
                ActorPoolSubsystem->GetPooledActor(Info.ActorClass, SpawnLocation, SpawnRot));

            if (!SpawnedEnemy)
            {
                UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to spawn from pool: %s"),
                    *GetName(), *Info.ActorClass->GetName());
                continue;
            }

            SpawnedEnemies.Add(SpawnedEnemy);

            if (SpawnedEnemy->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
            {
                SpawnedEnemy->OnReturnToSpawner.BindUObject(this, &AEnemyActorSpawner::UnregisterSpawnedEnemy);
            }
        }
    }
}

void AEnemyActorSpawner::UnregisterSpawnedEnemy(ABaseEnemy* Enemy)
{
    if (Enemy)
    {
        SpawnedEnemies.Remove(Enemy);
    }
}
