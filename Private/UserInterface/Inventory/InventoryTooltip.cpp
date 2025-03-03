// ©2024 - Happy Spirits Studios


#include "UserInterface/Inventory/InventoryTooltip.h"
#include "Items/ItemBase.h"
#include "UserInterface/Inventory/InventoryItemSlot.h"
#include "Components/TextBlock.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemHovered = InventorySlotHovered->GetItemReference();

	switch (ItemHovered->ItemData.ItemType)
	{
	case EItemType::Quest:
		ItemType->SetText(FText::FromString("Consumable"));

		// Do not need this visible
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		VisibilityValue->SetVisibility(ESlateVisibility::Collapsed);
		DistractionValue->SetVisibility(ESlateVisibility::Collapsed);
		HealValue->SetVisibility(ESlateVisibility::Collapsed);
		break;

	case EItemType::Armor:
		ItemType->SetText(FText::FromString("Consumable"));

		// Do not need this visible
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		VisibilityValue->SetVisibility(ESlateVisibility::Visible);
		DistractionValue->SetVisibility(ESlateVisibility::Collapsed);
		HealValue->SetVisibility(ESlateVisibility::Collapsed);
		break;

	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		
		// Do not need this visible
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		VisibilityValue->SetVisibility(ESlateVisibility::Collapsed);
		DistractionValue->SetVisibility(ESlateVisibility::Collapsed);
		HealValue->SetVisibility(ESlateVisibility::Visible);
		break;

	case EItemType::Throwable:
		ItemType->SetText(FText::FromString("Consumable"));

		// Do not need this visible
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		VisibilityValue->SetVisibility(ESlateVisibility::Collapsed);
		DistractionValue->SetVisibility(ESlateVisibility::Visible);
		HealValue->SetVisibility(ESlateVisibility::Collapsed);
		break;

	case EItemType::Valuable:
		ItemType->SetText(FText::FromString("Consumable"));

		// Do not need this visible
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		VisibilityValue->SetVisibility(ESlateVisibility::Collapsed);
		DistractionValue->SetVisibility(ESlateVisibility::Collapsed);
		HealValue->SetVisibility(ESlateVisibility::Collapsed);
		break;

	case EItemType::Weapon:
		ItemType->SetText(FText::FromString("Consumable"));

		// Do not need this visible
		DamageValue->SetVisibility(ESlateVisibility::Visible);
		VisibilityValue->SetVisibility(ESlateVisibility::Collapsed);
		DistractionValue->SetVisibility(ESlateVisibility::Collapsed);
		HealValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}

	ItemName->SetText(ItemHovered->ItemData.ItemText.Name);
	Description->SetText(ItemHovered->ItemData.ItemText.Description);
	DamageValue->SetText(FText::Format(FText::FromString("Damage: {0}"), FText::AsNumber(ItemHovered->ItemData.ItemStats.DamageValue)));
	HealValue->SetText(FText::Format(FText::FromString("Heal: {0}"), FText::AsNumber(ItemHovered->ItemData.ItemStats.HealValue)));
	VisibilityValue->SetText(FText::Format(FText::FromString("Visibility: {0}"), FText::AsNumber(ItemHovered->ItemData.ItemStats.VisibilityValue)));
	DistractionValue->SetText(FText::Format(FText::FromString("Distraction: {0}"), FText::AsNumber(ItemHovered->ItemData.ItemStats.DistractionValue)));
	SellValue->SetText(FText::Format(FText::FromString("Sell: {0}"), FText::AsNumber(ItemHovered->ItemData.ItemStats.SellValue)));

	const FString WeightText = { "Weight: " + FString::SanitizeFloat(ItemHovered->GetItemStackWeight())};
	Weight->SetText(FText::FromString(WeightText));
	Quantity->SetText(FText::Format(FText::FromString("Quantity: {0}"), FText::AsNumber(ItemHovered->Quantity)));
}
