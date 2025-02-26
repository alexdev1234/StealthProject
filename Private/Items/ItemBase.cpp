// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"
#include "UserInterface/Inventory/Component/InventoryComponent.h"

UItemBase::UItemBase():
	IsCopy(false),
	IsPickup(false)
{

}

UItemBase* UItemBase::CreateItemCopy() const
{
	UItemBase* newItem = NewObject<UItemBase>(StaticClass());

	newItem->ItemData.ID = this->ItemData.ID;
	newItem->Quantity = this->Quantity;
	newItem->ItemData = this->ItemData;
	newItem->IsCopy = true;

	return newItem;
}

void UItemBase::SetQuantity(const int32 newQuantity)
{
	if (newQuantity != Quantity)
	{
		// If stackable, clamp between 0 and max stacks, otherwise max is 1
		Quantity = FMath::Clamp(newQuantity, 0, ItemData.ItemNumeric.IsStackable ? ItemData.ItemNumeric.MaxStacks : 1);

		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningInventory->RemoveSingleInstanceOfItem(this);
			}
		}
	}
}

void UItemBase::UseItem(AStealthProjectCharacter* Character)
{
	// This should be implemented by each item type
}

void UItemBase::ResetItemFlags()
{
	IsCopy = false;
	IsPickup = false;
}
