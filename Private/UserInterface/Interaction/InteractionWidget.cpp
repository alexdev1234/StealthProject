// ©2024 - Happy Spirits Studios


#include "UserInterface/Interaction/InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KeyPressText->SetText(FText::FromString("Press"));
	CurrentInteractionDuration = 0.0f;
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) const
{
	// Set interaction text
	switch (InteractableData->InteractableType)
	{
	case EInteractableType::Pickup:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		if (InteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(FText::FromString("x{0}"), InteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;

	case EInteractableType::NonPlayerCharacter:
		break;

	case EInteractableType::Container:
		break;
		
	case EInteractableType::Device:
		break;

	case EInteractableType::Toggle:
		break;

	default:
		break;
	}

	NameText->SetText(InteractableData->Name);
	ActionText->SetText(InteractableData->Action);
}

float UInteractionWidget::UpdateInteractionProgress()
{
	/*TODO: Update some interactions to take a duration of time to complete before item has been interacted with*/
	return 0.0f;
}