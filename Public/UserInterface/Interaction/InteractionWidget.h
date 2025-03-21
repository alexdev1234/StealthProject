// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

struct FInteractableData;
class AStealthProjectCharacter;
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	AStealthProjectCharacter* PlayerCharacter;

	//=========================================================
	// Functions
	//=========================================================

	void UpdateWidget(const FInteractableData* InteractableData) const;

protected:
	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* NameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* ActionText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* QuantityText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* KeyPressText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UProgressBar* InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrentInteractionDuration;

	//=========================================================
	// Functions
	//=========================================================
	UFUNCTION(Category = "Interaction Widget | Interactable Data")
	float UpdateInteractionProgress();

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;
};
