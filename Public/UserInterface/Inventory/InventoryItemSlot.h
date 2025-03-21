// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UItemBase;
class UDragItemVisual;
class UInventoryTooltip;
class UTextBlock;
class UBorder;
class UImage;

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	//=========================================================
	// Functions
	//=========================================================
	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; };
	FORCEINLINE UItemBase* GetItemReference() const { return ItemReference; };

protected:
	//=========================================================
	// Functions
	//=========================================================
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(EditDefaultsOnly, Category="Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryTooltip> TooltipClass;

	UPROPERTY(VisibleAnywhere, Category="Inventory Slot")
	UItemBase* ItemReference;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UTextBlock* ItemQuantity;
};
