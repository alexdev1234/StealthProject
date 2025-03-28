// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveGameInstance.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API USaveGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//=========================================================
	// Functions
	//=========================================================
	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void LoadGame();

protected:
	//=========================================================
	// Functions
	//=========================================================

	virtual void Init() override;

	//=========================================================
	// Properties and Variables
	//=========================================================

	UPROPERTY(BlueprintReadWrite, Category = "SaveData | Level")
	FString CurrentLevelName;

	UPROPERTY(BlueprintReadWrite, Category = "SaveData | Inventory")
	UInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadWrite, Category = "SaveData | UserSettings")
	float VolumeSetting;

	UPROPERTY(BlueprintReadWrite, Category = "GameState")
	FIntPoint CurrentResolution;

private:
	//=========================================================
	// Properties and Variables
	//=========================================================

	static const FString SaveSlotName;
	static const int32 UserIndex;
};
