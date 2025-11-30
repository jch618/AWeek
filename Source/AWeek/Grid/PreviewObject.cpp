// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/PreviewObject.h"
#include "AWeek/Grid/GridTriggerBoxComponent.h"
#include "Components/BoxComponent.h"
#include "AWeek/Grid/GridPlacedActor.h"

// Sets default values
APreviewObject::APreviewObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_01.SM_Cube_01"));
	if (Mesh)
	{
		StaticMesh->SetStaticMesh(Mesh);
	}

	//Struct 크기 정하기

	//BoxComponent
	BoxComponent = CreateDefaultSubobject<UGridTriggerBoxComponent>(TEXT("UGridTriggerBoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionProfileName(TEXT("OverlapStructure"));
	BoxComponent->SetHiddenInGame(true);
	//BoxComponent 표시용 BoxMesh
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(RootComponent);
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxMesh->SetMobility(EComponentMobility::Movable);
	BoxMesh->SetCastShadow(false);

	//BoxMesh 설정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeRef(TEXT("/Engine/EngineMeshes/Cube.Cube"));
	if (CubeRef.Succeeded())
	{
		BoxMeshAsset = CubeRef.Object;
		BoxMesh->SetStaticMesh(BoxMeshAsset);
	}

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APreviewObject::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APreviewObject::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void APreviewObject::BeginPlay()
{
	Super::BeginPlay();
	/*if (MeshApply)
	{
		StaticMesh->SetStaticMesh(MeshApply);
	}
	StaticMesh->SetMaterial(0, TrueMaterial);*/

	/*BoxMesh->SetMaterial(0, TrueMaterial);*/
	UE_LOG(LogTemp, Log, TEXT("asdf"));
	if (Mesh)
	{
		StaticMesh->SetStaticMesh(Mesh);
	}
	StaticMesh->SetMaterial(0, TrueMaterial);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APreviewObject::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APreviewObject::OnBoxEndOverlap);
}
 
// Called every frame
void APreviewObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APreviewObject::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Rebuild();
}

#if WITH_EDITOR
void APreviewObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Rebuild();
}
#endif


void APreviewObject::CheckMaterial(bool value)
{
	//이벤트 작동할 때 만 확인하기
	StaticMesh->SetMaterial(0, value ? FalseMaterial : TrueMaterial);
	bCanPlace = !value;
}

void APreviewObject::Rebuild()
{
	if (!BoxComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("BOxComponent is Null!!"));
		return;
	}
	float CellX = FMath::Max(0.5, StructureSize.X);
	float CellY = FMath::Max(0.5, StructureSize.Y);
	float CellZ = FMath::Max(0.5, StructureSize.Z);

	const FVector DesiredSize(CellX * GridSize, CellY * GridSize, CellZ * GridSize);
	BoxComponent->SetBoxExtent(DesiredSize * 0.5f, true);
	BoxComponent->SetGenerateOverlapEvents(true);

	
	if (!BoxMesh->GetStaticMesh() && BoxMeshAsset)
	{
		UE_LOG(LogTemp, Log, TEXT("BoxMesh Component test!!!!!!!!!"));
		BoxMesh->SetStaticMesh(BoxMeshAsset);
		
	}

	if (bSizeDrivenByMesh)
	{
		SyncBoxComponentToBoxMesh();
		return;
	}

	if (UStaticMesh* SM = BoxMesh->GetStaticMesh())
	{
		//메쉬 실제 크기 
		const FVector BaseSize = SM->GetBounds().BoxExtent * 2.f;
		
		FVector Scale(1.f,1.f,1.f);
		if (BaseSize.X > 0.f) Scale.X = DesiredSize.X / BaseSize.X;
		if (BaseSize.Y > 0.f) Scale.Y = DesiredSize.Y / BaseSize.Y;
		if (BaseSize.Z > 0.f) Scale.Z = DesiredSize.Z / BaseSize.Z;
		BoxMesh->SetRelativeScale3D(Scale);
	}

	const float ZOffset = bBottomAlign ? (DesiredSize.Z * 0.5f) : 0.f;
	const FVector Offset(0.f, 0.f, ZOffset);
	BoxComponent->SetRelativeLocation(Offset);
	BoxMesh->SetRelativeLocation(Offset);

	
}


bool APreviewObject::PlaceActor(AGridPlacedActor* ParentGridPlacedActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("place Actor bCanPlace is %s"), bCanPlace ? TEXT("true") : TEXT("false"));
	/*if (bCanPlace)
	{
		//UE_LOG(LogTemp, Log, TEXT("place Actor bCanPlace is %s"), bCanPlace ? TEXT("true") : TEXT("false"));
		/*AGridPlacedActor* ChildPlacedActor = GetWorld()->SpawnActor<AGridPlacedActor>(BuildToPlace, GetActorLocation(), GetActorRotation());
		if (ParentGridPlacedActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Parent GridPlacedActor TEST Parent name is %s"), *ParentGridPlacedActor->GetName());
			UE_LOG(LogTemp, Log, TEXT("Child GridPlacedActor name is %s"), *ChildPlacedActor->GetName());
			ParentGridPlacedActor->BuildActor(ChildPlacedActor);
		}#1#

		APreviewObject* PlacedActor = GetWorld()->SpawnActor<APreviewObject>(BuildToPlace, GetActorLocation(), GetActorRotation());
		PlacedActor->SetActive(true);
	}else{return false;}
	return true;*/
	UE_LOG(LogTemp, Log,TEXT("1"));
	if (bCanPlace)
	{
		UE_LOG(LogTemp, Log,TEXT("2"));
		//UE_LOG(LogTemp, Log, TEXT("place Actor bCanPlace is %s"), bCanPlace ? TEXT("true") : TEXT("false"));
		AGridPlacedActor* ChildPlacedActor = GetWorld()->SpawnActor<AGridPlacedActor>(BuildToPlace, GetActorLocation(), GetActorRotation());
		if (ParentGridPlacedActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Parent GridPlacedActor TEST Parent name is %s"), *ParentGridPlacedActor->GetName());
			UE_LOG(LogTemp, Log, TEXT("Child GridPlacedActor name is %s"), *ChildPlacedActor->GetName());
			ParentGridPlacedActor->BuildActor(ChildPlacedActor);
		}
		UWorld* World = GetWorld();
		//WorldSubsystem 생기면 다시 하기
		/*if (UTestWorldSubsystem* TestWorldSubsystem_ = World->GetSubsystem<UTestWorldSubsystem>())
		{
			TestWorldSubsystem_->InsertBuilding(ChildPlacedActor);
			TestWorldSubsystem_->ReBuildingLink();
		}*/
	}else
	{
		UE_LOG(LogTemp, Log,TEXT("3"));
		return false;
	}
	UE_LOG(LogTemp, Log,TEXT("4"));
	return true;
}

void APreviewObject::SyncBoxComponentToBoxMesh()
{
	if (!BoxComponent || !BoxMesh || !BoxMesh->GetStaticMesh())return;

	const FBoxSphereBounds MeshworldBounds = BoxMesh->CalcBounds(BoxMesh->GetComponentTransform());

	BoxComponent->SetWorldLocation(MeshworldBounds.Origin);
	BoxComponent->SetBoxExtent(MeshworldBounds.BoxExtent, true);

	
}

void APreviewObject::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO Check Overlapped Object
	/*UE_LOG(LogTemp, Log, TEXT("a"));
	if (BlockObjects.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("b"));
		BoxMesh->SetMaterial(0, TrueMaterial);
		bCanPlace = true;
	}else
	{
		
	}*/
	
	UE_LOG(LogTemp, Log, TEXT("c"));
	BoxMesh->SetMaterial(0, FalseMaterial);
	BlockObjects.Add(OtherComp);
	bCanPlace = false;
}

void APreviewObject::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	/*//TODO Check Overlapped Object
	BlockObjects.Remove(OtherComp);
	if (BlockObjects.Num() == 0)
	{
		
	}*/
	BoxMesh->SetMaterial(0, TrueMaterial);
	bCanPlace = true;
}







