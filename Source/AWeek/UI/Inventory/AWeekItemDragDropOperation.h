#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "AWeekItemDragDropOperation.generated.h"

class UAWeekItemBase;
class UAWeekInventoryComponent;
struct FAWeekItemSlot;

UCLASS()
class AWEEK_API UAWeekItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	int32 ItemSlotIndex;

	UPROPERTY()
	TObjectPtr<UAWeekInventoryComponent> SourceInventory;
};
