// ©2024 - Happy Spirits Studios


#include "UserInterface/Inventory/InventoryPanel.h"
#include "UserInterface/Inventory/Component/InventoryComponent.h"
#include "UserInterface/MenuOperations/ItemDragDropOperation.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "StealthProject/StealthProjectCharacter.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AStealthProjectCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on Inventory Panel."));

		return true;
	}

	return false;
}

void UInventoryPanel::SetInfoText() const
{
	const FString WeightText = { "Weight: " + FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + " / " + FString::SanitizeFloat(InventoryReference->GetWeightCapacity())};
	WeightInfo->SetText(FText::FromString(WeightText));

	CapacityInfo->SetText(FText::Format(FText::FromString("Slots: {0} / {1}"), InventoryReference->GetInventoryContents().Num(), InventoryReference->GetSlotsCapacity()));
}

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		// Clear inventory
		InventoryPanel->ClearChildren();

		// Update the inventory
		for (UItemBase* const& item : InventoryReference->GetInventoryContents())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
			ItemSlot->SetItemReference(item);
			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}

		// Update carrying weight of player character
		PlayerCharacter = Cast<AStealthProjectCharacter>(GetOwningPlayerPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->SetWeight(InventoryReference->GetInventoryTotalWeight());
		}

		SetInfoText();
	}
}