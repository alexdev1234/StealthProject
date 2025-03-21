// ©2024 - Happy Spirits Studios


#include "UserInterface/Inventory/InGameMenu.h"
#include "StealthProject/StealthProjectCharacter.h"
#include "UserInterface/MenuOperations/ItemDragDropOperation.h"
#include "Items/ItemBase.h"

void UInGameMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInGameMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AStealthProjectCharacter>(GetOwningPlayerPawn());
}

bool UInGameMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}

	return false;
}

