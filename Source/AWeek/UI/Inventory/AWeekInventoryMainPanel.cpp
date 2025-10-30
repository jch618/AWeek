#include "AWeek/UI/Inventory/AWeekInventoryMainPanel.h"
#include "AWeek/UI/Inventory/AWeekInventoryPanel.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"

#include "CommonUIExtensions.h"

#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"

void UAWeekInventoryMainPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ensure(InventoryPanelClass);
}

void UAWeekInventoryMainPanel::NativeConstruct()
{
	Super::NativeConstruct();

	//if (InventoryPanelClass)
	//{
	//	ChestInventoryPanel = Cast<UAWeekInventoryPanel, UCommonActivatableWidget>(
	//		UCommonUIExtensions::PushContentToLayer_ForPlayer(GetOwningLocalPlayer(),
	//		FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryPanelClass));
	//	ChestInventoryPanel->DeactivateWidget();
	//}
}

// bool UAWeekInventoryMainPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("MainMenu: Drop Detected"));
// 	const UAWeekItemDragDropOperation* ItemDragDrop = Cast<UAWeekItemDragDropOperation>(InOperation);
//
// 	if (!ItemDragDrop->SourceInventory->IsValidItemSlotIndex(ItemDragDrop->ItemSlotIndex))
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Invalid ItemSlotIndex: %d"), ItemDragDrop->ItemSlotIndex);
// 		return false;
// 	}
// 	const FAWeekInventorySlotData& ItemSlot = ItemDragDrop->SourceInventory->GetItemSlotAt(ItemDragDrop->ItemSlotIndex);
// 	if (PlayerCharacter && !ItemSlot.bIsEmpty)
// 	{
// 		//PlayerCharacter->DropItem(ItemDragDrop->SourceItemSlot, ItemDragDrop->SourceItemSlot->Item->Quantity);
// 		PlayerCharacter->DropItemFromItemSlot(ItemSlot, ItemSlot.Item->Quantity);
// 		return true;
// 	}
// 	return false;
// }

UAWeekInventoryComponent* UAWeekInventoryMainPanel::GetPlayerInventoryComponent() const
{
	return PlayerInventoryPanel->GetInventory();
}

UAWeekInventoryComponent* UAWeekInventoryMainPanel::GetChestInventoryComponent() const
{
	return ChestInventoryPanel->GetInventory();
}

void UAWeekInventoryMainPanel::InitializeInventoryMainPanel()
{
	UE_LOG(LogTemp, Warning, TEXT("%s: call"), *FString(__FUNCTION__));
	
	PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn());
	PlayerInventoryPanel->LinkToInventory(PlayerCharacter->GetPlayerInventoryComponent(), PlayerCharacter);
	ChestInventoryPanel->SetVisibility(ESlateVisibility::Collapsed);

}

void UAWeekInventoryMainPanel::ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> InventoryComponent)
{
	if (ChestInventoryPanel)
	{
		ChestInventoryPanel->LinkToInventory(InventoryComponent);
		ChestInventoryPanel->SetVisibility(ESlateVisibility::Visible);
		bIsChestOpen = true;
	}
}

void UAWeekInventoryMainPanel::DeActivateChestInventory()
{
	if (ChestInventoryPanel)
	{
		ChestInventoryPanel->UnlinkFromInventory();
		ChestInventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
		bIsChestOpen = false;
	}
}
