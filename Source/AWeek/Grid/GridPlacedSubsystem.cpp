// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/GridPlacedSubsystem.h"

#include "CommonUIExtensions.h"
#include "AWeek/UI/AWeekActivatableWidget.h"
#include "AWeek/UI/Building/BuildingWheelPanel.h"
#include "AWeek/UI/MainWidget/Panel/BuildingCraftPanel.h"
#include "AWeek/UI/Building/PreviewObjectWidget.h"
#include "BuildingArea.h"

void UGridPlacedSubsystem::StartPlacement(TSubclassOf<APreviewObject> PreviewClass, APlayerController* ForPC, UBuildingWheelPanel* CraftPanel)
{
    if (GridWidgetClass == nullptr)
    {
        static const TCHAR* GridUIPath = TEXT("/Game/Grid/GridUI/WBP_PreviewObjectWidget.WBP_PreviewObjectWidget_C");
        GridWidgetClass = TSoftClassPtr<UPreviewObjectWidget>(FSoftClassPath(GridUIPath));
    }
    if (!PreviewClass)
    {
        UE_LOG(LogTemp, Log, TEXT("Preview Object class is null"));
    }
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Log, TEXT("GetWorld() is null"));
    }
    if (!ForPC)
    {
        UE_LOG(LogTemp, Log, TEXT("ForPC is null"));
    }
    if (!GetWorld() || !PreviewClass || !ForPC) return;
    UE_LOG(LogTemp, Log, TEXT("Preview Object4"));
    // 기존 세션 종료
    StopPlacement();
    BuildingCraftPanel = CraftPanel;
    OwnerPC = ForPC;

    // 프리뷰 스폰
    APreviewObject* NewPreview = GetWorld()->SpawnActor<APreviewObject>(PreviewClass);
    if (!NewPreview) return;

    PreviewActor = NewPreview;
    bActive = true;
    UE_LOG(LogTemp, Log, TEXT("Preview Object5"));
    ActiveBuildingGrid(true);
    //UIWidget 스폰
    if (UClass* Cls = GridWidgetClass.LoadSynchronous())   
    {
        EnsureGridUIShown(ForPC, Cls);                    
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load GridWidgetClass from path."));
    }
    
}

//Right Click
void UGridPlacedSubsystem::StopPlacement()
{
    bActive = false;
    ParentUnderCursor = nullptr;
    
    if (PreviewActor.IsValid())
    {
        PreviewActor->Destroy();
        PreviewActor = nullptr;
    }
    if (BuildingCraftPanel.IsValid())
    {
        //BuildingCraftPanel->OnDeactivated();
        BuildingCraftPanel.Reset();
    }
    ActiveBuildingGrid(false);
    HideGridUI();
}
//LeftClick
void UGridPlacedSubsystem::ConfirmPlacement()
{
    
    if (!PreviewActor.IsValid()) return;
    
    PreviewActor->PlaceActor(ParentUnderCursor.Get());
    PlacedActors.Add(ParentUnderCursor.Get());
    //TODO Inventory Item Remove
    BuildingCraftPanel->RemoveItem();
    
    StopPlacement();
}

//R
void UGridPlacedSubsystem::RotatePreview(float YawStepDeg)
{
    if (!PreviewActor.IsValid()) return;
    FRotator R = PreviewActor->GetActorRotation();
    R.Yaw += YawStepDeg;
    PreviewActor->SetActorRotation(R);
}

FVector UGridPlacedSubsystem::SnapToGrid(const FVector& P) const
{
    return FVector(
        FMath::GridSnap(P.X, GridSize),
        FMath::GridSnap(P.Y, GridSize),
        FMath::GridSnap(P.Z, GridSize));
}

bool UGridPlacedSubsystem::UpdatePreview()
{
    if (!OwnerPC.IsValid() || !PreviewActor.IsValid()) return false;

    // 화면 중앙에서 월드 레이
    int32 VX, VY;
    OwnerPC->GetViewportSize(VX, VY);

    FVector WorldLoc, WorldDir;
    if (!OwnerPC->DeprojectScreenPositionToWorld(VX/2.f, VY/2.f, WorldLoc, WorldDir)) return false;

    FVector End = WorldLoc + WorldDir * TraceDistance;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerPC->GetPawn());

    if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLoc, End, ECC_Visibility, Params))
    {
        FVector Snapped = SnapToGrid(Hit.ImpactPoint);
        PreviewActor->SetActorLocation(Snapped);

        // 바닥 붙이기 + 부모 그리드 감지
        // (필요시 채널/거리 조정)
        FVector Start = Snapped + FVector(0,0,1000);
        FVector Down = Start + FVector(0,0,-3000);
        FHitResult FloorHit;
        FCollisionQueryParams P2;
        P2.AddIgnoredActor(PreviewActor.Get());

        if (GetWorld()->LineTraceSingleByChannel(FloorHit, Start, Down, ECC_WorldStatic, P2))
        {
            // 높이 보정 (간단 버전)
            FVector L = PreviewActor->GetActorLocation();
            L.Z = FloorHit.ImpactPoint.Z;
            PreviewActor->SetActorLocation(L);

            AActor* A = FloorHit.GetActor();
            ParentUnderCursor = (A && A->IsA(AGridPlacedActor::StaticClass()))
                ? Cast<AGridPlacedActor>(A) : nullptr;
        }
        return true;
    }
    return false;
}

void UGridPlacedSubsystem::Tick(float DeltaTime)
{
    if (!bActive) return;
    UpdatePreview();
}

void UGridPlacedSubsystem::EnsureGridUIShown(APlayerController* PC, TSubclassOf<UPreviewObjectWidget> GridUIClass)
{
    
    if (!PC) return;

    if (ULocalPlayer* LP = PC->GetLocalPlayer())
    {
        auto* Inst = UCommonUIExtensions::PushContentToLayer_ForPlayer(
            LP, FGameplayTag::RequestGameplayTag(TEXT("UI.Layer.GameMenu")), GridUIClass);
        GridUI = Cast<UPreviewObjectWidget>(Inst);  // 캐시
    }
}

void UGridPlacedSubsystem::HideGridUI()
{
    if (GridUI.IsValid())
    {
        GridUI->DeactivateWidget();         // 숨김 (필요하면 Remove/Pop도 가능)
    }
}

void UGridPlacedSubsystem::ActiveBuildingGrid(bool bCheck)
{
    if (BuildingArea)
    {
        BuildingArea->ActiveArea(bCheck);
    }
    /*for (AGridPlacedActor* Actor : PlacedActors)
    {
        Actor->ChangeGridView(bCheck);
    }*/
}

void UGridPlacedSubsystem::RemoveActor(AGridPlacedActor* Actor)
{
    PlacedActors.Remove(Actor);
}



