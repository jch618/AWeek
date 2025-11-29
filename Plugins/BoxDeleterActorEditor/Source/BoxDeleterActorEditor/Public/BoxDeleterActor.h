#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BoxDeleterActor.generated.h"

UCLASS()
class BOXDELETERACTOREDITOR_API ABoxDeleterActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoxDeleterActor();
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Editor")
	void DeleteActorsInBox();
#endif
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Component")
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere, Category = "Editor")
	TArray<TSubclassOf<AActor>> FilteredClasses;
	UPROPERTY(EditAnywhere, Category = "Editor")
	TArray<AActor*> FilteredActors;

	//Z Position Filtering
	UPROPERTY(EditAnywhere, Category = "Editor")
	bool bEnableZFilter = false;
	UPROPERTY(EditAnywhere, Category = "Editor", meta = (EditCondition = "bEnableZFilter"))
	float MinZ = 0.f;

};
