#include "BaseEnemy.h"
#include "BaseEnemyAIController.h"
#include "../AWeekCharacterAnimInstance.h"

ABaseEnemy::ABaseEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    m_tryCleanUpCount = 0;
}

void ABaseEnemy::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    m_AIController = Cast<ABaseEnemyAIController>(NewController);
    if (!m_AIController)
        UE_LOG(LogTemp, Log, TEXT("AIC is Empty"));
}

void ABaseEnemy::BeginPlay()
{
    Super::BeginPlay();

    m_AnimInstance = Cast<UAWeekCharacterAnimInstance>(GetMesh()->GetAnimInstance());

    if (ABaseEnemy* DefaultEnemy = Cast<ABaseEnemy>(GetClass()->GetDefaultObject()))
    {
        StatDataAsset = DefaultEnemy->StatDataAsset;
    }

    if (!StatDataAsset)
    {
        StatDataAsset = Cast<UEnemyStatDataAsset>(
            StaticLoadObject(UEnemyStatDataAsset::StaticClass(), nullptr,
                TEXT("/Game/Enemies/Data/DA_EnemyStat.DA_EnemyStat"))
        );

        if (!StatDataAsset)
        {
            UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to load default StatDataAsset"), *GetName());
            return;
        }
    }

    m_Stat = StatDataAsset->DayData;

    auto& MessageSubsystem = UGameEventMessageSubsystem::Get(this);

    DayChangedListenerHandle = MessageSubsystem.RegisterListener<FDayChangedMessage>(
        FGameplayTag::RequestGameplayTag(FName("Event.DayChanged")),
        [this](FGameplayTag Channel, const FDayChangedMessage& Msg)
        {
            this->OnDayChanged(Msg);
        }
    );
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (DayChangedListenerHandle.IsValid())
    {
        DayChangedListenerHandle.Unregister();
    }

    Super::EndPlay(EndPlayReason);
}

void ABaseEnemy::OnDayChanged(const FDayChangedMessage& Msg)
{
    if (Msg.bIsDay)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] NightEnd!! Day: %d"), *GetName(), Msg.Day);
        m_Stat = StatDataAsset->DayData;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] Night Begin!! Day: %d"), *GetName(), Msg.Day);
        m_Stat = StatDataAsset->NightData;
    }

    // AI Perception Range Change
    if (ABaseEnemyAIController* AICon = Cast<ABaseEnemyAIController>(GetController()))
    {
        if (!AICon->SightConfig || !AICon->PerceptionComp)
            return;

        AICon->ChangeAISightRange(
            m_Stat.SightRadius,
            m_Stat.LoseSightRadius,
            m_Stat.VisionAngleDegrees,
            m_Stat.MaxAgeSeconds
        );
    }
}

void ABaseEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABaseEnemy::GetMoveSpeed_Code(EMovementType type) const
{
    return m_Stat.MoveSpeedMap[type];
}

float ABaseEnemy::GetMontagePlayRate_Code() const
{
    return m_Stat.AnimPlayRate;
}

void ABaseEnemy::DropItemBox()
{
    if (!LootChestClass) return;
    FVector Offset(0.f, 0.f, -100.f);
    FVector DropLocation = GetActorLocation() + Offset;
    AAWeekLootChest* Chest = AAWeekLootChest::SpawnLootChest(
        LootChestClass,
        GetWorld(),
        DropLocation,
        LootTableRow,
        30.0f
    );
}

void ABaseEnemy::OnLoadFromPool_Implementation()
{
    m_tryCleanUpCount = 0;

    const FString ClassName = GetClass()->GetName();

    const FString EnemyFolder = FString::Printf(TEXT("Pool/%s"), *ClassName);
#if WITH_EDITOR
    SetFolderPath(FName(*EnemyFolder));

    // Controller FolderSetting
    if (Controller)
    {
        const FString ControllerFolder = FString::Printf(TEXT("Pool/%s/Controller"), *ClassName);
        Controller->SetFolderPath(FName(*ControllerFolder));
    }
#endif
}

void ABaseEnemy::OnStoreToPool_Implementation()
{
    if (OwnerSpawner)
    {
        OnReturnToSpawner.ExecuteIfBound(this);
    }
#if WITH_EDITOR
    const FString ClassName = GetClass()->GetName();
    const FString EnemyFolder = FString::Printf(TEXT("Pool/%s"), *ClassName);
    SetFolderPath(FName(*EnemyFolder));

    if (Controller)
    {
        const FString ControllerFolder = FString::Printf(TEXT("Pool/%s/Controller"), *ClassName);
        Controller->SetFolderPath(FName(*ControllerFolder));
    }
#endif
}
