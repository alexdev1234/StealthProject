// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataStructs.h"
#include "StealthProject/StealthProjectCharacter.h"
#include "ItemBase.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:

	//=========================================================
	// Properties and Variables
	//=========================================================
	//UPROPERTY()
	UInventoryComponent* OwningInventory;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	FItemData ItemData;

	bool IsCopy;
	bool IsPickup;

	//=========================================================
	// Functions
	//=========================================================
	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const
	{
		return Quantity * ItemData.ItemNumeric.Weight;
	};

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const
	{
		return ItemData.ItemNumeric.Weight;
	};

	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const
	{
		return Quantity == ItemData.ItemNumeric.MaxStacks;
	};

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 newQuantity);

	virtual void UseItem(AStealthProjectCharacter* Character);

	void ResetItemFlags();

protected:

	//=========================================================
	// Functions
	//=========================================================
	bool operator==(const FName& OtherID) const
	{
		return ItemData.ID == OtherID;
	};
};
