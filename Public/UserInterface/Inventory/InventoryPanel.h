// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class AStealthProjectCharacter;
class UWrapBox;
class UTextBlock;
class UInventoryComponent;
class UInventoryItemSlot;

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//=========================================================
	// Functions
	//=========================================================
	UFUNCTION()
	void RefreshInventory();

	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	AStealthProjectCharacter* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

protected:
	//=========================================================
	// Functions
	//=========================================================
	void SetInfoText() const;

	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
