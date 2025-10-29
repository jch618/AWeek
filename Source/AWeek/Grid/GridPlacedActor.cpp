// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPlacedActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "TimerManager.h"
#include "Field/FieldSystemObjects.h"

FTimerHandle TimerHandle2;

// Sets default values
AGridPlacedActor::AGridPlacedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	//StaticMesh 버전
	/*StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	StaticMesh->SetupAttachment(Root);
	if (MeshApply)
	{
		StaticMesh->SetStaticMesh(MeshApply);
	}*/

	GeoComponent = CreateDefaultSubobject<UGeometryCollectionComponent>("GeoComponent");
	GeoComponent->SetupAttachment(RootComponent);

	GeoComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	GeoComponent->SetSimulatePhysics(false);

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(Root);

	if (BoxMeshApply)
	{
		BoxMesh->SetStaticMesh(BoxMeshApply);
	}

	//StaticMesh->SetCollisionProfileName(TEXT("BlockAll"));
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
	if (GeometryCollection)
	{
		GeometryCollection->SetSimulatePhysics(false);
	}


	//Fracture 작동 테스트
	
	// GetWorldTimerManager().SetTimer(TimerHandle2, this, &AGridPlacedActor::BrokeStructure, 3.0f, false);
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

	//if (MeshApply) StaticMesh->SetStaticMesh(MeshApply);
	if (GeoComponent && GeoAsset)
	{
		GeoComponent->SetRestCollection(GeoAsset);
	}
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

void AGridPlacedActor::Damage(float Damage)
{
	if (!bActive) return;
	BuildingStat.CurrentHealth -= Damage;
	if (BuildingStat.CurrentHealth <= 0)
	{
		BrokeStructure();
	}
}

void AGridPlacedActor::BrokeStructure()
{
	bActive = false;
	UE_LOG(LogTemp, Log, TEXT("BrokeStructure start"));


	// 붕괴 가능 상태로 전환
	GeoComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GeoComponent->SetSimulatePhysics(true);
	GeoComponent->WakeAllRigidBodies();
	
	const FBoxSphereBounds B = GeoComponent->Bounds;
	const FVector Origin = B.Origin;
	const float Radius = B.SphereRadius * 1.2f; // 콜렉션보다 살짝 크게
	
	//    Chaos_ExternalClusterStrain: 클러스터에 균열을 가함
	//Magnitude = 부서지는 잔해 개수
	URadialFalloff* Strain = NewObject<URadialFalloff>();
	Strain->SetRadialFalloff(
		/*Magnitude=*/ 3500.f,
		/*MinRange=*/ 0.f,
		/*MaxRange=*/ Radius,
		/*Default=*/ 0.f,
		/*Radius=*/ Radius,
		/*Position=*/ Origin,
		/*Falloff=*/ EFieldFalloffType::Field_Falloff_Linear
	);
	GeoComponent->ApplyPhysicsField(
		/*Enabled=*/ true,
		EGeometryCollectionPhysicsTypeEnum::Chaos_ExternalClusterStrain,
		/*Meta=*/ nullptr,
		Strain
	);

	
	URadialVector* Linear = NewObject<URadialVector>();
	Linear->SetRadialVector(/*Magnitude=*/ 200.f, /*Position=*/ Origin);
	GeoComponent->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_LinearForce, nullptr, Linear);

	URadialVector* Torque = NewObject<URadialVector>();
	Torque->SetRadialVector(/*Magnitude=*/ 200.f, /*Position=*/ Origin);
	GeoComponent->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_AngularTorque, nullptr, Torque);
	
	GeoComponent->AddRadialImpulse(Origin, Radius, /*Strength=*/ 200.f, ERadialImpulseFalloff::RIF_Linear, true);

	/*// 살짝 건드려서 실제로 무너지게
	const FVector Origin = GeoComponent->GetComponentLocation();
	GeoComponent->AddRadialImpulse(Origin, 300.f, 1200.f, ERadialImpulseFalloff::RIF_Linear, true);*/

	GetWorldTimerManager().SetTimer(TimerHandle2, this, &AGridPlacedActor::CleanupAfterBreak, 3.0f, false);

	
}

void AGridPlacedActor::Rebuild()
{
	if (!BoxComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("BOxComponent is Null!!"));
		return;
	}
	float CellX = FMath::Max(0.5, BuildingSize.Size.X);
	float CellY = FMath::Max(0.5, BuildingSize.Size.Y);
	float CellZ = FMath::Max(0.5, BuildingSize.Size.Z);

	const FVector DesiredSize(CellX * GridSize, CellY * GridSize, CellZ * GridSize);
	BoxComponent->SetBoxExtent(DesiredSize * 0.5f, true);
}

void AGridPlacedActor::CleanupAfterBreak()
{
	GeoComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Destroy();
}

FVector AGridPlacedActor::GetLinkWorldPosition()
{
	if (!GeoComponent)
	{
		return FVector::ZeroVector;
	}
	GeoComponent->UpdateBounds();
	const FBoxSphereBounds& Box = GeoComponent->Bounds;
	
	return  FVector(Box.Origin.X, Box.Origin.Y, Box.BoxExtent.Z +Box.Origin.Z);
}





