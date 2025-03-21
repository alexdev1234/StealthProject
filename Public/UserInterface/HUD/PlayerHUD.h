// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

struct FInteractableData;
class UInGameMenu;
class UInteractionWidget;
class UInventoryPanel;

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//=========================================================
	// Properties and Variables
	//=========================================================

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInGameMenu> InGameMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;

	//=========================================================
	// Functions
	//=========================================================
	APlayerHUD();

	void DisplayInventoryMenu();
	void HideInventoryMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

protected:
	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY()
	UInGameMenu* InGameMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	//=========================================================
	// Functions
	//=========================================================
	virtual void BeginPlay();
};
