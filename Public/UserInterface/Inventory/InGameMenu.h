// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.generated.h"

class AStealthProjectCharacter;
/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY()
	AStealthProjectCharacter* PlayerCharacter;

protected:
	//=========================================================
	// Functions
	//=========================================================
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
