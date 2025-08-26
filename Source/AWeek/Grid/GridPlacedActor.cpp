// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPlacedActor.h"

// Sets default values
AGridPlacedActor::AGridPlacedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	StaticMesh->SetupAttachment(Root);
	if (MeshApply)
	{
		StaticMesh->SetStaticMesh(MeshApply);
	}

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(Root);

	if (BoxMeshApply)
	{
		BoxMesh->SetStaticMesh(BoxMeshApply);
	}

	StaticMesh->SetCollisionProfileName(TEXT("BlockAll"));
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GridObjectSizeBoxComponent"));
	BoxComponent->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGridPlacedActor::BeginPlay()
{
	Super::BeginPlay();
	if (MeshApply)
	{
		StaticMesh->SetStaticMesh(MeshApply);
	}
}

// Called every frame
void AGridPlacedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridPlacedActor::OnConstruction(const FTransform& Transform)
{
	//에디터에서 수정되는 순간, Details에서 UPROPERTY 값을 수정할 때 실행
	Super::OnConstruction(Transform);

	if (MeshApply) StaticMesh->SetStaticMesh(MeshApply);
	if (BoxMeshApply) BoxMesh->SetStaticMesh(BoxMeshApply);
	if (Material) BoxMesh->SetMaterial(0, Material);
	if (BoxComponent)
	{
		MatchBoxToMesh(BoxComponent, BoxMesh);
	}
	
}

void AGridPlacedActor::MatchBoxToMesh(UBoxComponent* BoxComponent_, UStaticMeshComponent* BoxMesh_)
{
	if (!BoxComponent_ || !BoxMesh_ || !BoxMesh_->GetStaticMesh()) return;

	// 1) 메시의 "자산 로컬" 바운즈 (피벗 기준)
	const FBoxSphereBounds AssetBounds = BoxMesh_->GetStaticMesh()->GetBounds();
	const FVector LocalCenter = AssetBounds.Origin;   // 메시 로컬공간의 중심
	const FVector LocalExtent = AssetBounds.BoxExtent; // 메시 로컬공간의 반크기(half-size)

	// 2) 메시의 "월드" 트랜스폼
	const FTransform MeshXf = BoxMesh_->GetComponentTransform();

	// 3) 박스의 "목표 월드" 값 계산
	const FVector TargetWorldLocation = MeshXf.TransformPosition(LocalCenter); // 중심까지 이동
	const FRotator TargetWorldRotation = MeshXf.GetRotation().Rotator();
	const FVector TargetWorldScale   = MeshXf.GetScale3D();                    // 메시와 같은 스케일

	// 4) 박스 트랜스폼 맞추기
	//    (주의: BoxComponent가 Mesh의 '부모'라면, 부모 스케일 변경이 자식(메시)에 다시 영향)
	//    그런 경우 아래 SetWorldScale3D는 빼고, 아래의 "스케일 보정 방식"을 사용하세요.
	BoxComponent_->SetWorldLocationAndRotation(TargetWorldLocation, TargetWorldRotation);
	BoxComponent_->SetWorldScale3D(TargetWorldScale);

	// 5) UBoxComponent의 BoxExtent는 "스케일 적용 전 로컬 half-size" 값입니다.
	//    메시 로컬 half-size 그대로 넣으면, 위에서 맞춘 스케일과 회전이 곱해져 월드에서 정확히 일치합니다.
	BoxComponent_->SetBoxExtent(LocalExtent, /*bUpdateOverlaps=*/true);
}

void AGridPlacedActor::BuildActor(AGridPlacedActor* Actor)
{
	GridPlacedActors.Add(Actor);
	Actor->ParentGridPlacedActor = this;
}

void AGridPlacedActor::NotifyChildDestroyed(class AGridPlacedActor* ChildActor, int32 SlotIndex)
{
	
}
