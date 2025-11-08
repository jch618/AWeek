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
	// UE_LOG(LogTemp, Warning, TEXT("%s: call"), *FString(__FUNCTION__));
	
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
