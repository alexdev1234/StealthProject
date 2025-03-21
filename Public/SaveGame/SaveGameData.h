// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UserInterface/Inventory/Component/InventoryComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "SaveGameData.generated.h"


/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API USaveGameData : public USaveGame
{
	GENERATED_BODY()
	

public:
	USaveGameData() :
		CurrentLevelName(TEXT("DefaultLevel")),
		Inventory(nullptr),
		VolumeSetting(1.0f),
		ScreenResolutionX(1920),
		ScreenResolutionY(1080),
		GraphicsQuality(1),
		IsFullscreen(EWindowMode::Fullscreen)
	{};

	// Getters
	FORCEINLINE FString GetCurrentLevel() { return CurrentLevelName; };
	FORCEINLINE UInventoryComponent* GetPlayerInventory() { return Inventory; };
	FORCEINLINE float GetVolumeSetting() { return VolumeSetting; };
	FORCEINLINE int32 GetScreenResX() { return ScreenResolutionX; };
	FORCEINLINE int32 GetScreenResY() { return ScreenResolutionY; };
	FORCEINLINE int32 GetGraphicsQuality() { return GraphicsQuality; };
	FORCEINLINE int GetIsFullScreen() { return IsFullscreen; };

	// Setters
	FORCEINLINE void SetCurrentLevel(FString Level) { CurrentLevelName = Level; };
	FORCEINLINE void SetPlayerInventory(UInventoryComponent* newInventory) { Inventory = newInventory; };
	FORCEINLINE void SetVolumeSetting(float Volume) { VolumeSetting = Volume; };
	FORCEINLINE void SetScreenResX(int32 resX) { ScreenResolutionX = resX; };
	FORCEINLINE void SetScreenResY(int32 resY) { ScreenResolutionY = resY; };
	FORCEINLINE void SetGraphicsQuality(int32 graphics) { GraphicsQuality = graphics; };
	FORCEINLINE void SetIsFullScreen(int fullscreen) { IsFullscreen = fullscreen; };

protected:
	UPROPERTY(BlueprintReadWrite, Category = "SaveData | Level")
	FString CurrentLevelName;
	
	UPROPERTY(BlueprintReadWrite, Category = "SaveData | Inventory")
	UInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadWrite, Category = "SaveData | UserSettings")
	float VolumeSetting;

	UPROPERTY(BlueprintReadWrite, Category = "SaveData | GameSettings")
	int32 ScreenResolutionX;

	UPROPERTY(BlueprintReadWrite, Category = "SaveData | GameSettings")
	int32 ScreenResolutionY;

	UPROPERTY(BlueprintReadWrite, Category = "SaveData | GameSettings")
	int32 GraphicsQuality;

	// An integer for access into the enum
	UPROPERTY(BlueprintReadWrite, Category = "SaveData | GameSettings")
	int IsFullscreen;
};
