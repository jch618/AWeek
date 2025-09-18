// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Grid/LinkBoxComponent.h"
#include "Components/SceneComponent.h"
#include "LinkComponent.generated.h"


class UNavLinkCustomComponent;
class ULinkComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API ULinkComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULinkComponent();
	UPROPERTY(BlueprintReadWrite)
	ULinkBoxComponent* LinkBox;
	UPROPERTY(EditAnywhere)
	UNavLinkCustomComponent* NavLink;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
/*#if WITH_EDITOR
	virtual void OnComponentCreated() override;
#endif
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport) override;*/

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetPosition(FVector StartPosition, FVector EndPosition);
	bool bSetActive = true;

	/*UFUNCTION(BlueprintCallable)
	void OnReachedLink(AActor* MovingActor, const FVector& Destination);*/

		
};
