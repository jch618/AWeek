#include "BaseEnemy.h"
#include"BaseEnemyAIController.h"
#include"../AWeekCharacterAnimInstance.h"

ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	m_AIController = Cast<ABaseEnemyAIController>(NewController);
	if (m_AIController == nullptr)
		UE_LOG(LogTemp, Log, TEXT("AIC is Empty"));
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	m_AnimInstance = Cast<UAWeekCharacterAnimInstance>(GetMesh()->GetAnimInstance());



}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

