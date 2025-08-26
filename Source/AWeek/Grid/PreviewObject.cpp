// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/PreviewObject.h"
#include "AWeek/Grid/GridTriggerBoxComponent.h"

// Sets default values
APreviewObject::APreviewObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/VREditor/BasicMeshes/SM_Cube_01.SM_Cube_01"));
	if (MeshApply)
	{
		StaticMesh->SetStaticMesh(MeshApply);
	}

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	if (BoxMeshApply)
	{
		BoxMesh->SetStaticMesh(BoxMeshApply);
	}
	BoxMesh->SetupAttachment(Root);

	BoxComponent = CreateDefaultSubobject<UGridTriggerBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(Root);
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

	BoxMesh->SetMaterial(0, TrueMaterial);
}
 
// Called every frame
void APreviewObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APreviewObject::CheckMaterial(bool value)
{
	//이벤트 작동할 때 만 확인하기
	if (!bActiveActor)
	{
		BoxMesh->SetMaterial(0, value ? FalseMaterial : TrueMaterial);
		bCanPlace = !value;
	}
}

bool APreviewObject::PlaceActor(AGridPlacedActor* ParentGridPlacedActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("place Actor bCanPlace is %s"), bCanPlace ? TEXT("true") : TEXT("false"));
	if (bCanPlace)
	{
		//UE_LOG(LogTemp, Log, TEXT("place Actor bCanPlace is %s"), bCanPlace ? TEXT("true") : TEXT("false"));
		/*AGridPlacedActor* ChildPlacedActor = GetWorld()->SpawnActor<AGridPlacedActor>(BuildToPlace, GetActorLocation(), GetActorRotation());
		if (ParentGridPlacedActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Parent GridPlacedActor TEST Parent name is %s"), *ParentGridPlacedActor->GetName());
			UE_LOG(LogTemp, Log, TEXT("Child GridPlacedActor name is %s"), *ChildPlacedActor->GetName());
			ParentGridPlacedActor->BuildActor(ChildPlacedActor);
		}*/

		APreviewObject* PlacedActor = GetWorld()->SpawnActor<APreviewObject>(BuildToPlace, GetActorLocation(), GetActorRotation());
		PlacedActor->SetActive(true);
	}else{return false;}
	return true;
}

void APreviewObject::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshApply) StaticMesh->SetStaticMesh(MeshApply);
	if (BoxMeshApply) BoxMesh->SetStaticMesh(BoxMeshApply);
	//if (TrueMaterial) BoxMesh->SetMaterial(0, TrueMaterial);
	//SetStaticMeshToWorldSize(BoxMesh, BuildingSize.Size);
	
	if (BoxMesh && BoxMesh->GetStaticMesh())
	{
		SetMeshToGridCells(BoxMesh, BuildingSize.Size, 50.0f);
	}
	if (BoxComponent)
	{
		MatchBoxToMesh(BoxComponent, BoxMesh);
	}
	
}

void APreviewObject::SetActive(bool bValue)
{
	bActiveActor = bValue;
	if (ActiveMaterial)
	BoxMesh->SetMaterial(0, ActiveMaterial);
}



//TODO FStruct에 있는 Size 크기로 동기화 하기
//Box컴포넌트가 여러개일 경우 어떻게?

//BoxComponent와 BoxMesh 크기 맞추기
void APreviewObject::MatchBoxToMesh(UBoxComponent* BoxComponent_, UStaticMeshComponent* BoxMesh_)
{
	//박스 Component와 BoxMesh 크기 맞추기
	if (!BoxComponent_ || !BoxMesh_ || !BoxMesh_->GetStaticMesh()) return;

	
	const FBoxSphereBounds AssetBounds = BoxMesh_->GetStaticMesh()->GetBounds();
	const FVector LocalCenter = AssetBounds.Origin;   // 메시 로컬공간의 중심
	const FVector LocalExtent = AssetBounds.BoxExtent; // 메시 로컬공간의 반크기(half-size)

	
	const FTransform MeshXf = BoxMesh_->GetComponentTransform();

	
	const FVector TargetWorldLocation = MeshXf.TransformPosition(LocalCenter); // 중심까지 이동
	const FRotator TargetWorldRotation = MeshXf.GetRotation().Rotator();
	const FVector TargetWorldScale   = MeshXf.GetScale3D();                    // 메시와 같은 스케일

	// 트랜스폼 맞추기
	
	BoxComponent_->SetWorldLocationAndRotation(TargetWorldLocation, TargetWorldRotation);
	BoxComponent_->SetWorldScale3D(TargetWorldScale);
	
	BoxComponent_->SetBoxExtent(LocalExtent, /*bUpdateOverlaps=*/true);
}



void APreviewObject::SetMeshToGridCells(UStaticMeshComponent* Comp, const FIntVector& Cells, float Unit)
{
	const FVector TargetSize(Unit * Cells.X, Unit * Cells.Y, Unit * Cells.Z);
	SetStaticMeshToWorldSize(Comp, TargetSize);
}
void APreviewObject::SetStaticMeshToWorldSize(UStaticMeshComponent* Comp, const FVector& TargetSize)
{
	if (!Comp) return;
	UStaticMesh* Mesh = Comp->GetStaticMesh();
	if (!Mesh) return;

	const FBoxSphereBounds Bounds = Mesh->GetBounds();   // 로컬 바운즈 얻기
	const FVector MeshSize = Bounds.BoxExtent * 2.f;     // GetBounds로 얻는건 전체 크기의 절반이라 전체 크기를 알기 위해 *2


	//월드에서 보이는 크기 = 메시의 원래 로컬 크기 X 적용할 스케일
	//스케일 = 목표 크기 % 원래 크기
	auto SafeDiv = [](double A, double B){ return (FMath::Abs(B) > KINDA_SMALL_NUMBER) ? (A / B) : 1.0; };
	//원래 크기가 0에 가까울 경우 0으로 나누는 일이 생기기에 KINDA_SMALL_NUMBER보다 작으면 0으로 나누지 않고 1.0반환
	FVector NewScale(
		SafeDiv(TargetSize.X, MeshSize.X),
		SafeDiv(TargetSize.Y, MeshSize.Y),
		SafeDiv(TargetSize.Z, MeshSize.Z)
	);
	UE_LOG(LogTemp, Log, TEXT("X : %.3f , Y = %.3f, Z = %.3f"), NewScale.X, NewScale.Y, NewScale.Z);
	if (NewScale.Z == 0) NewScale.Z = 0.001f; // 두께가 0이 되는거 방지
	Comp->SetWorldScale3D(NewScale);
}


