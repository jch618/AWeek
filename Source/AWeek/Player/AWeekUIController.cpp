// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Player/AWeekUIController.h"
#include "AWeek/UI/Inventory/AWeekInventoryMainPanel.h"
#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"
#include "AWeek/UI/Inventory/AWeekHeldItem.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "CommonUIExtensions.h"

void AAWeekUIController::BeginPlay()
{
	Super::BeginPlay();

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

void AAWeekUIController::Tick(float DeltaTime)
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

void AAWeekUIController::ShowMainPanel()
{
	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetLocalPlayer(),
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
	}
}
void AAWeekUIController::HideMainPanel()
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

void AAWeekUIController::ToggleMainPanel()
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

void AAWeekUIController::ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
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


void AAWeekUIController::ShowCrosshair()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AAWeekUIController::HideCrosshair()
{
	if (CrosshairWidget)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AAWeekUIController::ShowInteractionWidget()
{
	if (InteractionWidgetClass)
	{
		InteractionWidget = Cast<UAWeekInteractionWidget, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(GetLocalPlayer(),
				FGameplayTag::RequestGameplayTag("UI.Layer.Game"), InteractionWidgetClass));
	}
}
void AAWeekUIController::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->DeactivateWidget();
	}
}
void AAWeekUIController::UpdateInteractionWidget(const FAWeekInteractableData* InteractableData)
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

void AAWeekUIController::ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
{
	InventoryMainPanelWidget->ActivateChestInventory(ChestInventory);
}

void AAWeekUIController::DeactivateChestInventory()
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

bool AAWeekUIController::IsHoldingItem() const
{
	return IsValid(HeldItem);
}

void AAWeekUIController::HandleItemSlotLeftClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	// if isHolding item
	//		if heldItem == slotItem => add item to SlotItem as possible and hold remaining items
	//		else => put HeldItem and hold SlotItem
	// else
	//		hold SlotItem

	if (IsHoldingItem())
	{
		const FAWeekInventorySlotData& ClickedItemSlot = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
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

void AAWeekUIController::HandleItemSlotRightClick(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
{
	const FAWeekInventorySlotData& ClickedItemSlot = OwningInventory->GetItemSlotAt(ClickedItemSlotIndex);
	if (IsHoldingItem())
	{
		if (!ClickedItemSlot.bIsEmpty)
		{
			if (HeldItem->GetItemReference()->ID == ClickedItemSlot.Item->ID)
			{
				OwningInventory->RemoveAmountOfItem(ClickedItemSlotIndex, 1);
				HeldItem->SetHeldItemQuantity(HeldItem->GetItemReference()->Quantity + 1);
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

void AAWeekUIController::MergeItem(int32 ClickedItemSlotIndex, TObjectPtr<UAWeekInventoryComponent> OwningInventory)
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

void AAWeekUIController::CreateHeldItem(TObjectPtr<UAWeekItemBase> NewHeldItem, TObjectPtr<UAWeekInventoryComponent> SourceInventory, int32 SourceItemSlotIndex)
{
	if (HeldItemVisualClass)
	{
		UAWeekHeldItemVisual* HeldItemVisual = Cast<UAWeekHeldItemVisual>(CreateWidget(this, HeldItemVisualClass));
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

void AAWeekUIController::HandleItemSlotShiftLeftClick(const FAWeekInventorySlotData& ClickedItemSlot)
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