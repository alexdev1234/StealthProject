// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthProjectGameMode.h"
#include "StealthProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStealthProjectGameMode::AStealthProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
