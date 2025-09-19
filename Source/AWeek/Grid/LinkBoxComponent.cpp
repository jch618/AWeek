// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/LinkBoxComponent.h"

#include "NavigationSystem.h"
#include "NavigationSystemTypes.h"
#include "Navigation/NavLinkProxy.h"
#include "NavAreas/NavArea_Default.h"
#include "NavAreas/NavArea_Null.h"

#include "AWeek/Grid/GridPlacedActor.h"
#include "MovieSceneSequenceID.h"
#include "NavLinkCustomComponent.h"
#include "NavLinkComponent.h"


#include "DrawDebugHelpers.h"

ULinkBoxComponent::ULinkBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;     // <- 필수
	PrimaryComponentTick.bStartWithTickEnabled = true; // 권장
	SetComponentTickEnabled(true);   

	
	SetGenerateOverlapEvents(true);
	OnComponentBeginOverlap.RemoveDynamic(this, &ULinkBoxComponent::HandleBeginOverlap);
	OnComponentEndOverlap.RemoveDynamic(this, &ULinkBoxComponent::HandleEndOverlap);

	OnComponentBeginOverlap.AddDynamic(this, &ULinkBoxComponent::HandleBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this,   &ULinkBoxComponent::HandleEndOverlap);

	Link = CreateDefaultSubobject<UNavLinkComponent>(TEXT("Link"));
	Link->SetupAttachment(this);
}

void ULinkBoxComponent::BeginPlay()
{
	Super::BeginPlay();
	SetLink();
}


void ULinkBoxComponent::OnRegister()
{
	Super::OnRegister();
	

	/*
	UE_LOG(LogTemp, Log, TEXT("Register test1"));
	// NavLink 없으면 생성
	if (!NLink && GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("Register test2"));
		AActor* Owner = GetOwner();
		// 에디터 트랜잭션 플래그
		UNavLinkCustomComponent* NewLink =
			NewObject<UNavLinkCustomComponent>(Owner, TEXT("NavLink"), RF_Transactional);
		NewLink->CreationMethod = EComponentCreationMethod::Instance;

		Owner->AddInstanceComponent(NewLink);

#if WITH_EDITOR
		// 에디터에 표시/Undo 기록을 위해
		Owner->Modify();
		NewLink->OnComponentCreated();
#endif
		NewLink->RegisterComponent();

		// 초기 설정
		NewLink->SetNavigationRelevancy(true);
		NewLink->SetEnabled(true);

		// 로컬 좌표 세팅
		SetLinkPosition();
		NewLink->SetLinkData(LinkPosition, OwnerPosition, ENavLinkDirection::BothWays);

		NLink = NewLink;
	}
	else if (NLink && !NLink->IsRegistered())
	{
		NLink->RegisterComponent();
	}

	// 이미 있다면 링크 데이터만 최신화
	if (NLink)
	{
		SetLinkPosition();
		NLink->SetLinkData(LinkPosition, OwnerPosition, ENavLinkDirection::BothWays);
	}*/
}


/*void ULinkBoxComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	if (!NLink && GetOwner())
	{
		NLink = NewObject<UNavLinkCustomComponent>(GetOwner(), TEXT("NavLink"));
		GetOwner()->AddInstanceComponent(NLink);
		NLink->RegisterComponent();

		NLink->SetNavigationRelevancy(true);
		SetLinkPosition();
		NLink->SetLinkData(LinkPosition, OwnerPosition, ENavLinkDirection::BothWays);
		NLink->SetEnabled(true);
	}
}*/

void ULinkBoxComponent::OnUnregister()
{
	/*// 내가 만든 보조 컴포넌트 먼저 정리 (부모가 나일 수 있으니 Super 전에)
	if (NLink)
	{
		if (!NLink->IsBeingDestroyed())
		{
#if WITH_EDITOR
			// 에디터에서 잦은 언레지스터 왕복 때는 Destroy 대신 잠시 끄는 것도 방법
			// NLink->UnregisterComponent();
			if (AActor* Owner = GetOwner())
			{
				Owner->RemoveInstanceComponent(NLink);
			}
			NLink->DestroyComponent();
#else
			if (AActor* Owner = GetOwner())
			{
				Owner->RemoveInstanceComponent(NLink);
			}
			NLink->DestroyComponent();
#endif
		}
		NLink = nullptr; // ★ 반드시 널로!
	}*/

	Super::OnUnregister();
}

void ULinkBoxComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (LinkProxy)
	{
		LinkProxy->Destroy();
		LinkProxy = nullptr;
	}
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}


void ULinkBoxComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DebugDrawLink();
}

void ULinkBoxComponent::DebugDrawLink()
{
	UE_LOG(LogTemp, Log, TEXT("asd"));
	if (!LinkProxy)return;
	const FTransform& T = LinkProxy->GetActorTransform();

	// SetLink()에서 계산한 로컬 좌표 재사용
	const FVector StartW = T.TransformPosition(LinkPosition); // 로컬→월드
	const FVector EndW   = T.TransformPosition(OwnerPosition);

	/*UE_LOG(LogTemp, Log, TEXT("StartW=%s  EndW=%s"), *StartW.ToString(), *EndW.ToString());
	UE_LOG(LogTemp, Log, TEXT("StartW=%s  EndW=%s"), *LinkPosition.ToString(), *OwnerPosition.ToString());*/
	
	DrawDebugSphere(GetWorld(), LinkPosition, 14.f, 12, FColor::Purple, /*bPersistentLines=*/false, /*LifeTime=*/1.f);
	DrawDebugSphere(GetWorld(), OwnerPosition,   14.f, 12, FColor::Cyan,   false, 1.f);
	DrawDebugDirectionalArrow(GetWorld(), LinkPosition, OwnerPosition, 60.f, FColor::Magenta, false, 5.f, 0, 1.f);
}



void ULinkBoxComponent::SetLinkPosition()
{
	FTransform OwnerTransform = GetOwner()->GetActorTransform();
	
	if (bOverlap)
	{
		AGridPlacedActor* OverlappedActor = Cast<AGridPlacedActor>(PlacedActors[0]);
		//LinkPosition = OwnerTransform.InverseTransformPosition(OverlappedActor->GetLinkWorldPosition());


		LinkPosition = OverlappedActor->GetLinkWorldPosition();
	}else
	{
		//LinkPosition = OwnerTransform.InverseTransformPosition(this->GetComponentLocation());
		LinkPosition = GetComponentLocation();
	}
	if (!OwnerClass)
	{
		OwnerClass = Cast<AGridPlacedActor>(GetOwner());
	}
	//OwnerPosition = OwnerTransform.InverseTransformPosition(OwnerClass->GetLinkWorldPosition());
	OwnerPosition = OwnerClass->GetLinkWorldPosition();
}



void ULinkBoxComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
											   UPrimitiveComponent* OtherComp, int32 /*OtherBodyIndex*/,
											   bool bFromSweep, const FHitResult& /*SweepResult*/)
{
	if (!OtherActor || OtherActor == GetOwner()) return;
	//구조물의 사이즈 BoxComponent를 감지
	
	if (UBoxComponent* Box = Cast<UBoxComponent>(OtherComp))
	{
		if (Box->ComponentHasTag(TEXT("Test_Structure")))
		{
			bOverlap = true;
			PlacedActors.Add(OtherActor);
			if (PlacedActors.Num() == 1)
			{
				SetLinkPosition();
			}
		}
	}

	//Actor 단위 감지
	/*UE_LOG(LogTemp, Log, TEXT("[Box BeginOverlap] Owner=%s, OtherActor=%s, OtherComp=%s, FromSweep=%d"),
		*GetOwner()->GetName(),
		*OtherActor->GetName(),
		OtherComp ? *OtherComp->GetName() : TEXT("None"),
		bFromSweep ? 1 : 0);*/
	
}

void ULinkBoxComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
											 UPrimitiveComponent* OtherComp, int32 /*OtherBodyIndex*/)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	if (UBoxComponent* Box = Cast<UBoxComponent>(OtherActor->GetComponentByClass<UBoxComponent>()))
	{
		if (Box->ComponentHasTag(TEXT("Test_Structure")))
		{
			PlacedActors.Remove(OtherActor);
			if (PlacedActors.IsEmpty())
			{
				bOverlap = false;
				SetLinkPosition();
			}
		}
	}
	/*UE_LOG(LogTemp, Log, TEXT("[Box EndOverlap] Owner=%s, OtherActor=%s, OtherComp=%s"),
		*GetOwner()->GetName(),
		*OtherActor->GetName(),
		OtherComp ? *OtherComp->GetName() : TEXT("None"));*/

	
}

void ULinkBoxComponent::SetLink()
{
	UE_LOG(LogTemp, Log, TEXT("SetLink test1"));
	SetLinkPosition();
	if (!LinkProxy)
	{
		FTransform LinkTransform(FRotator::ZeroRotator, GetComponentLocation());
		LinkProxy = GetWorld()->SpawnActor<ANavLinkProxy>(ANavLinkProxy::StaticClass(),LinkTransform);
		UE_LOG(LogTemp, Log, TEXT("SetLink test2"));
		if (!LinkProxy)
		{
			UE_LOG(LogTemp, Log, TEXT("SetLink test3"));
			return;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("SetLink test4"));
	const FTransform& LinkT = LinkProxy->GetActorTransform();
	FVector LinkStart = LinkT.InverseTransformPosition(LinkPosition);
	FVector LinkEnd = LinkT.InverseTransformPosition(OwnerPosition);

	if (UNavLinkCustomComponent* LinkComp = LinkProxy->GetSmartLinkComp())
	{
		UE_LOG(LogTemp, Log, TEXT("SetLink test5"));
		LinkComp->SetLinkData(LinkStart, LinkEnd, ENavLinkDirection::BothWays);
		LinkComp->SetEnabledArea(UNavArea_Default::StaticClass());
		LinkComp->SetDisabledArea(UNavArea_Null::StaticClass());
		LinkComp->SetNavigationRelevancy(true);
	}
}


