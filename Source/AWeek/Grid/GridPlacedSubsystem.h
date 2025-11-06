// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AWeek/Grid/GridPlacedActor.h"
#include "AWeek/Grid/PreviewObject.h"

#include "GridPlacedSubsystem.generated.h"


/**
 * 
 */
class UAWeekActivatableWidget;
class UBuildingCraftPanel;
class UPreviewObjectWidget;
UCLASS()
class AWEEK_API UGridPlacedSubsystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
	// 진입/종료
	UFUNCTION(BlueprintCallable) void StartPlacement(TSubclassOf<APreviewObject> PreviewClass, APlayerController* ForPC, UBuildingCraftPanel* Panel);
	UFUNCTION(BlueprintCallable) void StopPlacement();

	// 사용자 액션
	UFUNCTION(BlueprintCallable) void ConfirmPlacement();
	UFUNCTION(BlueprintCallable) void RotatePreview(float YawStepDeg = 90.f);

	// 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float GridSize = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TraceDistance = 10000.f;

	// Tickable
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UGridPlacementSubsystem, STATGROUP_Tickables); }
	virtual bool IsTickable() const override { return bActive && PreviewActor != nullptr && OwnerPC.IsValid(); }

private:
	bool bActive = false;
	TWeakObjectPtr<APlayerController> OwnerPC;
	TWeakObjectPtr<APreviewObject> PreviewActor;
	TWeakObjectPtr<AGridPlacedActor> ParentUnderCursor;

	TSoftClassPtr<UAWeekActivatableWidget> GridWidgetClass;

	void EnsureGridUIShown(APlayerController* PC, TSubclassOf<UPreviewObjectWidget> GridUIClass);
	void HideGridUI();
	TWeakObjectPtr<UAWeekActivatableWidget> GridUI;
	TWeakObjectPtr<UBuildingCraftPanel> BuildingCraftPanel;
	
	bool UpdatePreview();                    // 디프로젝션+레이캐스트+스냅
	FVector SnapToGrid(const FVector& P) const;
	void SnapPreviewToSurface();
};
