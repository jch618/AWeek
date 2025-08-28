#include "AWeek/UI/AWeekMainMenu.h"
#include "AWeek/UI/Inventory/AWeekInventoryPanel.h"
#include "AWeek/UI/Inventory/AWeekItemDragDropOperation.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"

void UAWeekMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UAWeekMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn());
	PlayerInventoryPanel->LinkToInventory(PlayerCharacter->GetInventory(), PlayerCharacter);
	ChestInventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
}

bool UAWeekMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UAWeekItemDragDropOperation* ItemDragDrop = Cast<UAWeekItemDragDropOperation>(InOperation);

	if (!ItemDragDrop->SourceInventory->IsValidItemSlotIndex(ItemDragDrop->ItemSlotIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ItemSlotIndex: %d"), ItemDragDrop->ItemSlotIndex);
		return false;
	}
	const FItemSlot& ItemSlot = ItemDragDrop->SourceInventory->GetItemSlotAt(ItemDragDrop->ItemSlotIndex);
	if (PlayerCharacter && !ItemSlot.bIsEmpty)
	{
		//PlayerCharacter->DropItem(ItemDragDrop->SourceItemSlot, ItemDragDrop->SourceItemSlot->Item->Quantity);
		PlayerCharacter->DropItemFromItemSlot(ItemSlot, ItemSlot.Item->Quantity);
		return true;
	}
	return false;
}

void UAWeekMainMenu::ActivateChestInventory(TObjectPtr<UAWeekInventoryComponent> InventoryComponent)
{
	if (ChestInventoryPanel)
	{
		ChestInventoryPanel->LinkToInventory(InventoryComponent, nullptr);
		ChestInventoryPanel->SetVisibility(ESlateVisibility::Visible);
	}
}

void UAWeekMainMenu::DeActivateChestInventory()
{
	if (ChestInventoryPanel)
	{
		ChestInventoryPanel->UnlinkFromInventory();
		ChestInventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}
