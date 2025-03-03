// ©2024 - Happy Spirits Studios


#include "UserInterface/Inventory/Component/InventoryComponent.h"
#include "Items/ItemBase.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* Item)
{
	if (GetOwner())
	{
		if (Item)
		{
			const int32 initialAmount = Item->Quantity;

			// Handle non-stackable
			if (!Item->ItemData.ItemNumeric.IsStackable)
			{
				return HandleNonStackableItems(Item);
			}

			const int32 stackableAmount = HandleStackableItems(Item, initialAmount);

			if (stackableAmount == initialAmount)
			{
				return FItemAddResult::AddedAll(stackableAmount, FText::Format(FText::FromString("Added {0} {1} to inventory."), stackableAmount, Item->ItemData.ItemText.Name));
			}

			if (stackableAmount < initialAmount && stackableAmount > 0)
			{
				return FItemAddResult::AddedPartial(stackableAmount, FText::Format(FText::FromString("Added {0} {1} to inventory. Cannot add more to the stack."), Item->ItemData.ItemText.Name));
			}

			if (stackableAmount <= 0)
			{
				return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. Cannot add more to the stack."), Item->ItemData.ItemText.Name));
			}
		}
	}

	check(false);
	return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. CheckOwner() failed."), Item->ItemData.ItemText.Name));
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* Item) const
{
	if (!Item)
	{
		return nullptr;
	}

	if (Inventory.Contains(Item))
	{
		return Item;
	}

	return nullptr;
}

UItemBase* UInventoryComponent::FindNextItemByID(UItemBase* Item) const
{
	if (!Item)
	{
		return nullptr;
	}

	if (const TArray<TObjectPtr<UItemBase>>::ElementType* result = Inventory.FindByKey(Item))
	{
		return *result;
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* Item) const
{
	if (!Item)
	{
		return nullptr;
	}

	// Check if the item is in a full stack or not
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* result = Inventory.FindByPredicate([&Item](const UItemBase* InventoryItem)
		{
			return InventoryItem->ItemData.ID == Item->ItemData.ID && !InventoryItem->IsFullItemStack();
		}))
	{
		return *result;
	}

	return nullptr;
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* Item)
{
	if (!Item)
	{
		return;
	}

	Inventory.RemoveSingle(Item);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* Item, const int32 amount)
{
	if (!Item)
	{
		return 0;
	}

	const int32 amountToRemove = FMath::Min(amount, Item->Quantity);
	Item->SetQuantity(Item->Quantity - amountToRemove);

	Weight -= amount * Item->GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return amountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* Item, const int32 amount)
{
	if (!Item)
	{
		return;
	}

	// Make sure we can actually add to our inventory
	if (!Inventory.Num() + 1 >= SlotsCapacity)
	{
		RemoveAmountOfItem(Item, amount);
		AddNewItem(Item, amount);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(UItemBase* Item)
{
	// Check if we have a valid weight
	if (FMath::IsNearlyZero(Item->GetItemSingleWeight()) || Item->GetItemSingleWeight() < 0)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. Invalid weight value."), Item->ItemData.ItemText.Name));
	}

	// Will we overflow capacity?
	if (Weight + Item->GetItemSingleWeight() > GetWeightCapacity())
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. Overflowing weight capacity."), Item->ItemData.ItemText.Name));
	}

	// Will we overflow the inventory size?
	if (Inventory.Num() + 1 > SlotsCapacity)
	{
		return FItemAddResult::AddedNone(FText::Format(FText::FromString("Could not add {0} to inventory. All inventory slots are full."), Item->ItemData.ItemText.Name));
	}

	AddNewItem(Item, 1);
	return FItemAddResult::AddedAll(1, FText::Format(FText::FromString("Added {0} {1} to inventory."), 1, Item->ItemData.ItemText.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* Item, int32 requestedAmount)
{
	// Invalid item data
	if (!Item)
	{
		return 0;
	}

	// Invalid item data
	if (requestedAmount <= 0 || FMath::IsNearlyZero(Item->GetItemStackWeight()))
	{

		return 0;
	}

	int32 distributeAmount = requestedAmount;

	// Check if input item already exists in inventory and doesn't have a full stack
	UItemBase* existingItemStack = FindNextPartialStack(Item);

	// Distribute item over existing stacks
	while (existingItemStack)
	{
		// Determine how much is needed to make the next full stack
		const int32 amountToFullStack = CalculateNumberForFullStacks(existingItemStack, distributeAmount);

		// Determine how much is actually able to be carried by the player
		const int32 weightLimitAddAmount = CalculateWeightAmount(existingItemStack, amountToFullStack);

		// As long as remaining amount doesn't overflow weight capacity
		if (weightLimitAddAmount > 0)
		{
			existingItemStack->SetQuantity(existingItemStack->Quantity + weightLimitAddAmount);
			Weight += existingItemStack->GetItemSingleWeight() * weightLimitAddAmount;

			// Adjust count to be distributed
			distributeAmount -= weightLimitAddAmount;

			Item->SetQuantity(distributeAmount);

			if (Weight >= WeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return requestedAmount - distributeAmount;
			}
		}
		else if (weightLimitAddAmount <= 0)
		{
			if (distributeAmount != requestedAmount)
			{
				OnInventoryUpdated.Broadcast();
				return requestedAmount - distributeAmount;
			}

			return 0;
		}

		// All of item stack was added
		if (distributeAmount <= 0)
		{
			OnInventoryUpdated.Broadcast();
			return requestedAmount;
		}

		// Find the next stack
		existingItemStack = FindNextPartialStack(Item);
	}

	// No more partial stacks found. Check if we need to add a new stack
	if (Inventory.Num() + 1 <= SlotsCapacity)
	{
		const int32 weightLimitAddAmount = CalculateWeightAmount(Item, distributeAmount);

		if (weightLimitAddAmount > 0)
		{
			if (weightLimitAddAmount < distributeAmount)
			{
				distributeAmount -= weightLimitAddAmount;
				Item->SetQuantity(distributeAmount);

				AddNewItem(Item->CreateItemCopy(), weightLimitAddAmount);
				return requestedAmount - distributeAmount;
			}

			AddNewItem(Item, distributeAmount);
			return requestedAmount;
		}
	}

	OnInventoryUpdated.Broadcast();
	return requestedAmount - distributeAmount;
}

int32 UInventoryComponent::CalculateWeightAmount(UItemBase* Item, int32 requestedAmount)
{
	if (!Item)
	{
		return 0;
	}

	const int32 WeightAddedAmount = FMath::FloorToInt((GetWeightCapacity() - Weight) / Item->GetItemSingleWeight());
	if (WeightAddedAmount >= requestedAmount)
	{
		return requestedAmount;
	}

	return WeightAddedAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStacks(UItemBase* Item, int32 requestedAmount)
{
	if (!Item)
	{
		return 0;
	}

	const int32 StackAddedAmount = Item->ItemData.ItemNumeric.MaxStacks - Item->Quantity;

	return FMath::Min(requestedAmount, StackAddedAmount);
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 amount)
{
	if (!Item)
	{
		return;
	}

	UItemBase* newItem;

	if (Item->IsCopy || Item->IsPickup)
	{
		newItem = Item;
		newItem->ResetItemFlags();
	}
	else
	{
		newItem = Item->CreateItemCopy();
	}

	newItem->OwningInventory = this;
	newItem->Quantity = amount;

	Inventory.Add(newItem);
	Weight += newItem->GetItemStackWeight();

	UE_LOG(LogTemp, Warning, TEXT("Added item to inventory"));

	OnInventoryUpdated.Broadcast();
}
