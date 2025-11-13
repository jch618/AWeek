#include "ActorPoolSubSystem.h"
#include "IPoolable.h"
#include "Engine/World.h"
#include "UObject/UObjectIterator.h"
#include "UObject/UnrealType.h"

void UActorPoolSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("ActorPoolSubSystem Initialized"));
}

void UActorPoolSubSystem::Deinitialize()
{
    ActorPools.Empty();
    Super::Deinitialize();
}
void UActorPoolSubSystem::SpawnNewActors(TSubclassOf<AActor> ActorClass, int32 Count)
{
    if (!*ActorClass || Count <= 0) return;

    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*>& Pool = ActorPools.FindOrAdd(ActorClass);

    for (int32 i = 0; i < Count; ++i)
    {
        // SpawnActor는 이미 BP 초기값과 Components 세팅을 적용
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* NewActor = World->SpawnActor<AActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if (!NewActor) continue;

        // 풀용 기본 세팅
        NewActor->SetActorTickEnabled(false);
        NewActor->SetActorHiddenInGame(true);
        NewActor->SetActorEnableCollision(false);

        // Poolable Interface 호출
        if (NewActor->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
        {
            IPoolable::Execute_OnStoreToPool(NewActor);
        }

        Pool.Add(NewActor);
    }
}

AActor* UActorPoolSubSystem::GetPooledActor(TSubclassOf<AActor> ActorClass, FVector Location, FRotator Rotation, int32 NumToSpawnIfEmpty)
{
    if (!*ActorClass) return nullptr;

    TArray<AActor*>& Pool = ActorPools.FindOrAdd(ActorClass);

    if (Pool.Num() == 0)
    {
        SpawnNewActors(ActorClass, NumToSpawnIfEmpty);
    }

    if (Pool.Num() == 0) return nullptr;

    AActor* Result = Pool.Pop();

    // 위치/회전/활성화만 초기화
    Result->SetActorLocation(Location);
    Result->SetActorRotation(Rotation);
    Result->SetActorHiddenInGame(false);
    Result->SetActorEnableCollision(true);
    Result->SetActorTickEnabled(true);

    if (Result->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
    {
        IPoolable::Execute_OnLoadFromPool(Result);
    }

    return Result;
}

void UActorPoolSubSystem::ReturnActorToPool(AActor* Actor)
{
    if (!Actor) return;

    Actor->SetActorHiddenInGame(true);
    Actor->SetActorEnableCollision(false);
    Actor->SetActorTickEnabled(false);

    if (Actor->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
    {
        IPoolable::Execute_OnStoreToPool(Actor);
    }

    ActorPools.FindOrAdd(Actor->GetClass()).Add(Actor);
}
