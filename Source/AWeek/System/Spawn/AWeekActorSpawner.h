#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolumeActor.h"
#include "NavigationSystem.h"
#include "AWeekActorSpawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    int32 SpawnCount = 1;
};

UCLASS()
class AWEEK_API AAWeekActorSpawner : public AActor
{
    GENERATED_BODY()

public:
    AAWeekActorSpawner();

protected:
    virtual void BeginPlay() override;

public:
    //SpawnActorList
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<FSpawnInfo> SpawnList;
    // Allow/Deny Volume
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    TArray<ASpawnVolumeActor*> Volumes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    bool bSpawnActorRandomRotation = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    bool bUseVolumeWeight = true;

    UFUNCTION(CallInEditor, Category = "Spawn")
    void SpawnActor();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    bool bIsPathFinding = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    AActor* PathFindingTarget = nullptr;

#if WITH_EDITORONLY_DATA
    UPROPERTY(VisibleAnywhere, Category = "Spawn")
    TArray<AActor*> EditorSpawnedActors;
#endif

#if WITH_EDITOR
    // Spawn Actor in Editor For Test
    UFUNCTION(CallInEditor, Category = "Spawn")
    void ClearTestSpawnedActors();
#endif

protected:
    FVector GetRandomLocationInVolumes() const;
    const ASpawnVolumeActor* ChooseRandomAllowVolume() const;
};
