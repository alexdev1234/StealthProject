// ©2024 - Happy Spirits Studios

#include "UserInterface/Menus/Buttons/MenuButton.h"
#include "Components/TextBlock.h"

void UMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonName->SetText(FText::FromString("Button Name"));
}