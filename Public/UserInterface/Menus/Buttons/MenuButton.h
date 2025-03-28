// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButton.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UMenuButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(VisibleAnywhere, Category="Button", meta= (BindWidget))
	UButton* Button;

	UPROPERTY(VisibleAnywhere, Category="Button", meta= (BindWidget))
	UTextBlock* ButtonName;

	//=========================================================
	// Functions
	//=========================================================
	virtual void NativeConstruct() override;
};
