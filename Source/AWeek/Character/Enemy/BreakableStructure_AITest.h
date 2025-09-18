#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BreakableStructure_AITest.generated.h"

UCLASS(Blueprintable)
class AWEEK_API ABreakableStructure_AITest : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UNavModifierComponent* NavModifier;
	
public:	
	ABreakableStructure_AITest();
public:
	UFUNCTION(BlueprintCallable)
	virtual void StructureBreak();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;




};
