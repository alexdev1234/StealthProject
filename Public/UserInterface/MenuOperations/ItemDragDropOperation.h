// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UItemBase;
class UInventoryComponent;

UCLASS()
class STEALTHPROJECT_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UInventoryComponent* SourceInventory;
};
