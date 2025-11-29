#include "ActorPoolSubSystem.h"
#include "IPoolable.h"
#include "Engine/World.h"
#include "UObject/UObjectIterator.h"
#include "UObject/UnrealType.h"

static const FName TAG_IsPooled(TEXT("IsPooled"));

void UActorPoolSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
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
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* NewActor = World->SpawnActor<AActor>(
            ActorClass,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            SpawnParams
        );

        if (!NewActor) continue;

        //FolderPath
        ApplyPoolFolderPath(NewActor, ActorClass);

        //Add Pool Tags
        NewActor->Tags.AddUnique(TAG_IsPooled);

        //Initialize Options
        NewActor->SetActorTickEnabled(false);
        NewActor->SetActorHiddenInGame(true);
        NewActor->SetActorEnableCollision(false);

        // if Using IPoolable, call IPoolable Interface funciton
        if (NewActor->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
        {
            IPoolable::Execute_OnStoreToPool(NewActor);
        }

        Pool.Add(NewActor);
    }
}

AActor* UActorPoolSubSystem::GetPooledActor(TSubclassOf<AActor> ActorClass,FVector Location,FRotator Rotation,int32 NumToSpawnIfEmpty)
{
    if (!*ActorClass) return nullptr;

    TArray<AActor*>& Pool = ActorPools.FindOrAdd(ActorClass);

    if (Pool.Num() == 0)
    {
        SpawnNewActors(ActorClass, NumToSpawnIfEmpty);
    }

    if (Pool.Num() == 0) return nullptr;

    AActor* Result = Pool.Pop();

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

    const bool bIsPooledTag = Actor->Tags.Contains(TAG_IsPooled);

    if (!bIsPooledTag)
    {
        Actor->Destroy();
        return;
    }

    Actor->SetActorHiddenInGame(true);
    Actor->SetActorEnableCollision(false);
    Actor->SetActorTickEnabled(false);

    if (Actor->GetClass()->ImplementsInterface(UPoolable::StaticClass()))
    {
        IPoolable::Execute_OnStoreToPool(Actor);
    }

    ActorPools.FindOrAdd(Actor->GetClass()).Add(Actor);
}

FName UActorPoolSubSystem::GetPoolFolderPath(TSubclassOf<AActor> ActorClass) const
{
    return FName(*FString::Printf(TEXT("Pool/%s"), *ActorClass->GetName()));
}

void UActorPoolSubSystem::ApplyPoolFolderPath(AActor* Actor, TSubclassOf<AActor> ActorClass)
{
    if (!Actor) return;
#if WITH_EDITOR
    const FName FolderPath = GetPoolFolderPath(ActorClass);
    Actor->SetFolderPath(FolderPath);
#endif
    
}
