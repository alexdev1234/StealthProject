// ©2024 - Happy Spirits Studios


#include "UserInterface/HUD/PlayerHUD.h"
#include "UserInterface/Inventory/InGameMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"

APlayerHUD::APlayerHUD() : 
	bIsMenuVisible(false)
{

}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!InGameMenuWidget && InGameMenuClass)
	{
		InGameMenuWidget = CreateWidget<UInGameMenu>(GetWorld(), InGameMenuClass);
		InGameMenuWidget->AddToViewport(5);
		InGameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!InteractionWidget && InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::DisplayInventoryMenu()
{
	if (InGameMenuWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Showing inventory"));
		bIsMenuVisible = true;
		InGameMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideInventoryMenu()
{
	if (InGameMenuWidget)
	{
		bIsMenuVisible = false;
		InGameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		UE_LOG(LogTemp, Error, TEXT("Hiding inventory"));
	}
}

void APlayerHUD::ToggleMenu()
{
	// If visible, hide it and change to game only input
	if (bIsMenuVisible)
	{
		HideInventoryMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	// Otherwise, show the inventory
	else
	{
		DisplayInventoryMenu();

		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void APlayerHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		// If widget is not already visible, make it visible
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			ShowInteractionWidget();
		}
		InteractionWidget->UpdateWidget(InteractableData);
	}
}