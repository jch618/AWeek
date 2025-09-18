// game
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Player/AWeekPlayerController.h"

#include "AWeek/UI/Inventory/AWeekDragItemVisual.h"

void UAWeekDragItemVisual::NativeConstruct()
{
	//SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
}

void UAWeekDragItemVisual::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D MousePos;
	if (APlayerController* Controller = GetWorld()->GetFirstPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Native Tick"));
		Controller->GetMousePosition(MousePos.X, MousePos.Y);
		SetPositionInViewport(MousePos, false);
	}
}
