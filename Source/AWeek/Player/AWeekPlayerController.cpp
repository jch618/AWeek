// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerController.h"
#include "../UI/AWeekUserWidget.h"
#include "AWeek/UI/AWeekInventoryMainPanel.h"
#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/UI/Inventory/AWeekDragItemVisual.h"
#include "AWeek/UI/Inventory/AWeekHeldItem.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"

#include "CommonUIExtensions.h"

AAWeekPlayerController::AAWeekPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAWeekPlayerController::BeginPlay()
{
	Super::BeginPlay();

	mMainWidget = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Main.UI_Main_C'"));

	if (IsValid(mMainWidget))
	{
		UUserWidget* MainWidget = CreateWidget<UUserWidget>(this,
			mMainWidget);

		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}

	check(InventoryMainPanelClass);
	check(InteractionWidgetClass);

	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetLocalPlayer(),
			FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
		InventoryMainPanelWidget->InitializeInventoryMainPanel();
		InventoryMainPanelWidget->DeactivateWidget();
	}
}

void AAWeekPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Manually update the held item's visual position to follow the cursor
	if (IsHoldingItem() && InventoryMainPanelWidget->IsActivated())
	{
		FVector2D MousePosition;
		if (GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			MousePosition += FVector2D(5.0f, 5.0f);
			HeldItem->UpdateHeldVisualPosition(MousePosition);
		}
	}
}

// =====================================================
// INVENTORY SYSTEM
// =====================================================

void AAWeekPlayerController::ShowMainPanel()
{
	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetLocalPlayer(),
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
	}
}
void AAWeekPlayerController::HideMainPanel()
{
	if (InventoryMainPanelWidget)
	{
		InventoryMainPanelWidget->DeactivateWidget();
		if (IsHoldingItem())
		{
			HeldItem->ReturnHeldItemToInventory();
			HeldItem = nullptr;
		}
	}
}

void AAWeekPlayerController::ToggleMainPanel()
{
	if (!IsValid(InventoryMainPanelWidget) || !InventoryMainPanelWidget->IsActivated())
	{
		ShowMainPanel();
		SetShowMouseCursor(true);
	}
	else
	{
		if (InventoryMainPanelWidget->IsChestOpen())
		{
			DeactivateChestInventory();
		}
		HideMainPanel();
		SetShowMouseCursor(false);
	}
}

void AAWeekPlayerController::ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
{
	if (!InventoryMainPanelWidget->IsChestOpen())
	{
		if (!IsValid(InventoryMainPanelWidget) || !InventoryMainPanelWidget->IsActivated())
		{
			ShowMainPanel();
			SetShowMouseCursor(true);
		}
		ActivateChestInventory(ChestInventory);
	}
	else
	{
		DeactivateChestInventory();
		HideMainPanel();
		SetShowMouseCursor(false);
	}
}


void AAWeekPlayerController::ShowCrosshair()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAWeekPlayerController::HideCrosshair()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AAWeekPlayerController::ShowInteractionWidget()
{
	if (InteractionWidgetClass)
	{
		InteractionWidget = Cast<UAWeekInteractionWidget, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetLocalPlayer(),
				FGameplayTag::RequestGameplayTag("UI.Layer.Game"), InteractionWidgetClass));
	}
}
void AAWeekPlayerController::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->DeactivateWidget();
	}
}
void AAWeekPlayerController::UpdateInteractionWidget(const FAWeekInteractableData* InteractableData)
{
	if (InteractionWidget == nullptr || !InteractionWidget->IsActivated())
	{
		if (InteractionWidgetClass)
		{
			ShowInteractionWidget();
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void AAWeekPlayerController::ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
{
	InventoryMainPanelWidget->ActivateChestInventory(ChestInventory);
}

void AAWeekPlayerController::DeactivateChestInventory()
{
	if (IsHoldingItem())
	{
		if (HeldItem->GetSourceInventory() == InventoryMainPanelWidget->GetChestInventoryComponent())
		{
			HeldItem->ReturnHeldItemToInventory();
			HeldItem = nullptr;
		}
	}
	InventoryMainPanelWidget->DeActivateChestInventory();
}

bool AAWeekPlayerController::IsHoldingItem() const
{
	return IsValid(HeldItem);
}

void AAWeekPlayerController::HandleItemSlotLeftClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	// if isHolding item
	//		if heldItem == slotItem => add item to SlotItem as possible and hold remaining items
	//		else => put HeldItem and hold SlotItem
	// else
	//		hold SlotItem

	if (IsHoldingItem())
	{
		const FAWeekItemSlot& ClickedItemSlot = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
		if (ClickedItemSlot.bIsEmpty)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Place Item to slot and clear HeldItem"), *FString(__FUNCTION__));

			OwningInventory->PlaceItemAt(HeldItem->ReleaseHeldItem(), ClickedItemSlotIndex);
			HeldItem->ClearHeldItem();
			HeldItem = nullptr;
		}
		else
		{
			// if target slot item is same as clicked item, merge item quantity as possible and hold remaining quantity of item 
			if (ClickedItemSlot.Item->ID == HeldItem->GetItemReference()->ID)
			{
				MergeItem(ClickedItemSlotIndex, OwningInventory);
			}
			// put held item to target slot and hold target slot item
			else
			{
				UAWeekItemBase* ClickedItem = OwningInventory->ReleaseItemAt(ClickedItemSlotIndex);
				OwningInventory->PlaceItemAt(HeldItem->ReleaseHeldItem(), ClickedItemSlotIndex);
				HeldItem->SetItemReference(ClickedItem);
			}
		}
	}
	else
	{
		if (!OwningInventory->GetItemSlotAt(ClickedItemSlotIndex).bIsEmpty)
		{
			UAWeekItemBase* NewHeldItem = OwningInventory->ReleaseItemAt(ClickedItemSlotIndex);
			CreateHeldItem(NewHeldItem, OwningInventory, ClickedItemSlotIndex);
		}
	}
}

void AAWeekPlayerController::HandleItemSlotRightClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	const FAWeekItemSlot& ClickedItemSlot = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
	if (IsHoldingItem())
	{
		if (ClickedItemSlot.bIsEmpty)
		{
			//if (HeldItem->GetItemReference()->Quantity == 1)
			//{
			//	OwningInventory->PlaceItemAt(HeldItem->ReleaseHeldItem(), ClickedItemSlotIndex);
			//	HeldItem->ClearHeldItem();
			//	HeldItem = nullptr;
			//}
			//else
			//{
			//	UAWeekItemBase* NewItemSlotItem = HeldItem->GetItemReference()->CreateItemCopy();
			//	NewItemSlotItem->SetQuantity(1);
			//	HeldItem->SetHeldItemQuantity(HeldItem->GetItemReference()->Quantity - 1);
			//	OwningInventory->PlaceItemAt(NewItemSlotItem, ClickedItemSlotIndex);
			//}
		}
		else
		{
			if (HeldItem->GetItemReference()->ID == ClickedItemSlot.Item->ID)
			{
				OwningInventory->RemoveAmountOfItem(ClickedItemSlotIndex, 1);
				HeldItem->SetHeldItemQuantity(HeldItem->GetItemReference()->Quantity + 1);
				//OwningInventory->AddItemQuantityAt(ClickedItemSlotIndex, 1);
				//HeldItem->SetHeldItemQuantity(HeldItem->GetItemReference()->Quantity - 1);
				//if (HeldItem->IsEmpty())
				//{
				//	HeldItem = nullptr;
				//}
			}
		}
	}
	else
	{
		if (!ClickedItemSlot.bIsEmpty)
		{
			// hold one item
			UAWeekItemBase* NewHeldItem = ClickedItemSlot.Item->CreateItemCopy();
			NewHeldItem->SetQuantity(1);
			CreateHeldItem(NewHeldItem, OwningInventory, ClickedItemSlotIndex);
			OwningInventory->RemoveAmountOfItem(ClickedItemSlotIndex, 1);
		}
	}
}

void AAWeekPlayerController::HoldItemAt(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{


}

void AAWeekPlayerController::PutItemTo(int32 TargetSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{

}

void AAWeekPlayerController::SwapItem()
{
}

void AAWeekPlayerController::MergeItem(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Merge Item"), *FString(__FUNCTION__));

	int32 HeldItemQuantity = HeldItem->GetItemReference()->Quantity;
	int32 ActualAmountAdded = OwningInventory->AddItemQuantityAt(ClickedItemSlotIndex, HeldItemQuantity);
	HeldItemQuantity -= ActualAmountAdded;
	HeldItem->SetHeldItemQuantity(HeldItemQuantity);
	if (HeldItem->IsEmpty())
	{
		HeldItem = nullptr;
	}
}

void AAWeekPlayerController::CreateHeldItem(TObjectPtr<UAWeekItemBase> NewHeldItem, TObjectPtr<UAWeekInventoryComponent> SourceInventory, int32 SourceItemSlotIndex)
{
	if (HeldItemVisualClass)
	{
		UAWeekDragItemVisual* HeldItemVisual = Cast<UAWeekDragItemVisual>(CreateWidget(this, HeldItemVisualClass));
		HeldItem = NewObject<UAWeekHeldItem>();

		if (HeldItemVisual)
		{
			FHeldItemData HeldItemData;
			HeldItemData.HeldItemVisual = HeldItemVisual;
			HeldItemData.ItemReference = NewHeldItem;
			HeldItemData.SourceInventory = SourceInventory;
			HeldItemData.SourceSlotIndex = SourceItemSlotIndex;

			HeldItem->InitializeHeldItem(HeldItemData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: HeldItemVisual is nullptr!"), *FString(__FUNCTION__));
		}
	}
}

void AAWeekPlayerController::HandleItemSlotShiftLeftClick(const FAWeekItemSlot& ClickedItemSlot)
{

	if (!ClickedItemSlot.bIsEmpty && InventoryMainPanelWidget->IsChestOpen())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: execute"), *FString(__FUNCTION__));
		UAWeekInventoryComponent* SourceInventory = ClickedItemSlot.OwningInventory;
		UAWeekInventoryComponent* TargetInventory = nullptr;
		if (SourceInventory == InventoryMainPanelWidget->GetPlayerInventoryComponent())
		{
			TargetInventory = InventoryMainPanelWidget->GetChestInventoryComponent();
		}
		else
		{
			TargetInventory = InventoryMainPanelWidget->GetPlayerInventoryComponent();
		}
		if (SourceInventory && TargetInventory)
		{
			if (SourceInventory->IsValidItemSlotIndex(ClickedItemSlot.ItemSlotIndex))
			{
				FAWeekItemAddResult AddResult = TargetInventory->HandleAddItem(ClickedItemSlot.Item->CreateItemCopy());
				SourceInventory->RemoveAmountOfItem(ClickedItemSlot.ItemSlotIndex, AddResult.ActualAmountAdded);

			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s: Source Inventory or TargetInventory is null."), *FString(__FUNCTION__));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Chest is not open"), *FString(__FUNCTION__));
	}
}