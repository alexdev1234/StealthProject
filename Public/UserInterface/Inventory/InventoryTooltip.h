// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTooltip.generated.h"

class UInventoryItemSlot;
class UTextBlock;

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()
public:
	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(VisibleAnywhere)
	UInventoryItemSlot* InventorySlotHovered;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemType;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* VisibilityValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DistractionValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SellValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Weight;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Quantity;


protected:
	//=========================================================
	// Functions
	//=========================================================
	virtual void NativeConstruct() override;
};
