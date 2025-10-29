#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"../../System/IDamageAble.h"
#include"../../System/GameEventMessageSubsystem.h"
#include"../../System/AWeekEventMessageInfo.h"
#include"EnemyStatDataAsset.h"
#include "BaseEnemy.generated.h"


UCLASS()
class AWEEK_API ABaseEnemy : public ACharacter, public IDamageAble
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	class ABaseEnemyAIController* m_AIController;
	UPROPERTY(BlueprintReadWrite)
	class UAWeekCharacterAnimInstance* m_AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "BaseEnemy")
	FEnemyStatData m_Stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseEnemy")
	UEnemyStatDataAsset* StatDataAsset;
	
public:
	ABaseEnemy();
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void OnDayChanged(const FDayChangedMessage& Msg);
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed_Code(EMovementType type) const;
	UFUNCTION(BlueprintCallable)
	float GetMontagePlayRate_Code() const;
private:
	FGameEventMessageListenerHandle DayChangedListenerHandle;

};
