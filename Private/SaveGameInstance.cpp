// ©2024 - Happy Spirits Studios


#include "SaveGameInstance.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SaveGameData.h"
#include "StealthProject/StealthProjectCharacter.h"
#include "UserInterface/Inventory/Component/InventoryComponent.h"

const FString USaveGameInstance::SaveSlotName = TEXT("GameSaveData");
const int32 USaveGameInstance::UserIndex = 0;

void USaveGameInstance::Init()
{
	Super::Init();

	// Load game if save game data exists
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("Loading from save game..."));
		LoadGame();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No save game found. Starting new game."));

		// Set default values for new game
		CurrentLevelName = TEXT("DefaultLevel");
		VolumeSetting = 1.0f;
		
		// Apply default settings
		UGameUserSettings* userSettings = UGameUserSettings::GetGameUserSettings();
		if (userSettings)
		{
			userSettings->RunHardwareBenchmark();
			userSettings->ApplyHardwareBenchmarkResults();
			userSettings->SetScreenResolution(userSettings->GetDesktopResolution());
			userSettings->SetFullscreenMode(EWindowMode::Fullscreen);
			userSettings->ApplySettings(false);
		}
	}
}

void USaveGameInstance::SaveGame()
{
	USaveGameData* SaveGameInstance = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create save game instance!"));
		return;
	}

	// Populate save data with custom made data
	SaveGameInstance->SetCurrentLevel(CurrentLevelName);
	SaveGameInstance->SetPlayerInventory(Inventory);
	SaveGameInstance->SetVolumeSetting(VolumeSetting);

	// TODO: Is there any more data that we want to save here?

	// Retrieve and save game user settings
	UGameUserSettings* userSettings = UGameUserSettings::GetGameUserSettings();
	if (userSettings)
	{
		FIntPoint Resolution = userSettings->GetScreenResolution();
		SaveGameInstance->SetScreenResX(Resolution.X);
		SaveGameInstance->SetScreenResY(Resolution.Y);
		SaveGameInstance->SetGraphicsQuality(userSettings->GetOverallScalabilityLevel());
		SaveGameInstance->SetIsFullScreen(userSettings->GetFullscreenMode());

		// TODO: Add more data that we want to save for graphics settings
	}

	// Save to slot
	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("Game saved successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save game."));
	}
}

void USaveGameInstance::LoadGame()
{
	// Check if save game exists
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("No save game found."));
		return;
	}

	// Load save game object
	USaveGameData* LoadGameInstance = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	if (!LoadGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load save game instance."));
		return;
	}

	// Restore save data
	CurrentLevelName = LoadGameInstance->GetCurrentLevel();
	Inventory = LoadGameInstance->GetPlayerInventory();
	VolumeSetting = LoadGameInstance->GetVolumeSetting();
	CurrentResolution = FIntPoint{ LoadGameInstance->GetScreenResX(), LoadGameInstance->GetScreenResY() };

	// Apply loaded game user settings
	UGameUserSettings* userSettings = UGameUserSettings::GetGameUserSettings();
	if (userSettings)
	{
		userSettings->SetScreenResolution(CurrentResolution);
		userSettings->SetOverallScalabilityLevel(LoadGameInstance->GetGraphicsQuality());

		/**
		* Game window fullscreen mode
		*	0 = Fullscreen
		*	1 = Windowed fullscreen
		*	2 = Windowed
		*/

		switch (LoadGameInstance->GetIsFullScreen())
		{
		case 0:
			userSettings->SetFullscreenMode(EWindowMode::Fullscreen);
			break;

		case 1:
			userSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
			break;

		case 2:
			userSettings->SetFullscreenMode(EWindowMode::Windowed);
			break;
		}

		userSettings->ApplySettings(false);
	}

	UE_LOG(LogTemp, Log, TEXT("Game loaded successfully. Current level: %s"), *CurrentLevelName);

	// Load level
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));

	// Get player after level loads to set inventory
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, LoadGameInstance]()
		{
			// Get player character
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController)
			{
				APawn* PlayerPawn = PlayerController->GetPawn();
				AStealthProjectCharacter* player = Cast<AStealthProjectCharacter>(PlayerPawn);

				if (player)
				{
					UE_LOG(LogTemp, Log, TEXT("Player character loaded: %s"), *player->GetName());

					// Set the inventory of the player
					player->SetInventory(Inventory);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Player character not found."));
				}
			}
		}, 1.0f, false);
}
