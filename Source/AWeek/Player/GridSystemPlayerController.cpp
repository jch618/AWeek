// Fill out your copyright notice in the Description page of Project Settings.


#include "GridSystemPlayerController.h"

void AGridSystemPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AGridSystemPlayerController::HandleLeftClick);
	InputComponent->BindAction("RotatePreviewActor", IE_Pressed, this, &AGridSystemPlayerController::RotatePreviewActor);
}

void AGridSystemPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (BuildToPlaceObject)
	{
		PreviewActor = GetWorld()->SpawnActor<APreviewObject>(BuildToPlaceObject);
	}
}

void AGridSystemPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//화면 중앙을 기준으로
	FVector WorldLocation, WorldDirection;
	int32 ViewportX, ViewportY;
	GetViewportSize(ViewportX, ViewportY);
	DeprojectScreenPositionToWorld(ViewportX / 2, ViewportY / 2, WorldLocation, WorldDirection);

	//Preview 오브젝트 표시 최대 사거리 10000.0f (줄여도 됨)
	FVector End = WorldLocation + WorldDirection * 10000.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());

	if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, End, ECC_Visibility, Params))
	{
		FVector SnappedLocation = SnapToGrid(Hit.ImpactPoint);

		if (PreviewActor)
		{
			PreviewActor->SetActorLocation(SnappedLocation);
			SnapPreviewToSurface();
			
		}
	}
}

void AGridSystemPlayerController::HandleLeftClick()
{
	UE_LOG(LogTemp, Log, TEXT("Left CLick accept!!"));
	if (PreviewActor)
	{
		UE_LOG(LogTemp, Log, TEXT("Create Actor %s"), *PreviewActor->GetName());
		//PreviewActor에 저장되어 있는 Building 설처
		PreviewActor->PlaceActor(ParentGridPlacedActor);
	}
}

void AGridSystemPlayerController::RotatePreviewActor()
{
	if (PreviewActor)
	{
		FRotator CurrentRotation = PreviewActor->GetActorRotation();
		FRotator NewRotation = CurrentRotation + FRotator(0.f, 90.f, 0.f);
		PreviewActor->SetActorRotation(NewRotation);
	}
}

FVector AGridSystemPlayerController::GetSnappedLocation(const FVector& WorldLocation) const
{
	FVector Snapped;
	Snapped.X = FMath::GridSnap(WorldLocation.X, GridSize);
	Snapped.Y = FMath::GridSnap(WorldLocation.Y, GridSize);
	Snapped.Z = FMath::GridSnap(WorldLocation.Z, GridSize); // 또는 고정 Z 값

	return Snapped;
}

FVector AGridSystemPlayerController::SnapToGrid(const FVector& Location)
{
	return FVector(
	FMath::GridSnap(Location.X, GridSize),
	FMath::GridSnap(Location.Y, GridSize),
	FMath::GridSnap(Location.Z, GridSize) // 나중에 Z 값 고정할거면 여기서
);
}

void AGridSystemPlayerController::SnapPreviewToSurface()
{
	if (!PreviewActor){return;}

	FVector StartLocation = PreviewActor->GetActorLocation() + FVector(0.0f, 0.0f, 1000.0f);
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -3000.0f); //아래로 Raycast
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PreviewActor);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_WorldStatic, Params))
	{
		FVector HitLocation = HitResult.ImpactPoint;
		FVector Normal = HitResult.ImpactNormal;

		//Actor의 높이 절반을 기준으로 위로 올리기(?)
		FBox Box = PreviewActor->GetComponentsBoundingBox();
		float ActorBottomZ = Box.Min.Z;
		float CurrentZ = PreviewActor->GetActorLocation().Z;

		float OffsetZ = HitLocation.Z - ActorBottomZ;

		FVector FinalLocation = PreviewActor->GetActorLocation();
		FinalLocation.Z += OffsetZ;

		PreviewActor->SetActorLocation(FinalLocation);
		AActor* HitActor = HitResult.GetActor();
		
		if (HitActor && HitActor->IsA(AGridPlacedActor::StaticClass()))
		{
			ParentGridPlacedActor = Cast<AGridPlacedActor>(HitActor);
		}else{ParentGridPlacedActor = nullptr;}
	}
}






