#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"../../System/IDamageAble.h"
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

public:
	ABaseEnemy();
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
