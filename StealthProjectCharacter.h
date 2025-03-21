// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Public/Interfaces/InteractionInterface.h"
#include "StealthProjectCharacter.generated.h"

class USpringArmComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class APlayerHUD;
class UInventoryComponent;
class UItemBase;
class UAIPerceptionStimuliSourceComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

// Info about interaction
USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY();

	FInteractionData():
		CurrentInteractable(nullptr),
		LastInteractionCheckTime(0.0f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS(config=Game)
class AStealthProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera Sprint Arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Toggle Inventory Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleInventoryAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;
	
public:
	//========================================================
	// Functions
	//========================================================
	AStealthProjectCharacter();
	
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	FORCEINLINE bool IsInteracting() const
	{
		return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction);
	};

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; };
	FORCEINLINE void SetInventory(UInventoryComponent* newInventory) { PlayerInventory = newInventory; };

	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* Item, const int32 Quantity);

	//------- Stealth Mechanics --------//
	// This will need to be calculated every tick since this is dependant on a number of light sources
	// TODO: Figure out way to make this so we don't need to calculate every tick (this is a waste of resources)
	void CalculateVisibility();

	FORCEINLINE void SetWeight(float newWeight) { Weight = newWeight; }

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	//=========================================================
	// Properties and Variables
	//=========================================================

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool ToggleCrouch = true; // Will be set by user settings once that is setup
	bool IsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool ToggleSprint = false; // Will be set by user settings once that is setup
	bool IsSprinting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed", meta = (AllowPrivateAccess = "true"))
	float CrouchSpeed = 150.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	// Interaction Frequency
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float InteractionCheckFrequency;

	// Interaction Distance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	float InteractDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	// Player HUD
	UPROPERTY()
	APlayerHUD* HUD;

	// Inventory
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent* PlayerInventory;

	// Stealth Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stealth")
	float Visibility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stealth")
	float Weight;

	// Player should be able to be seen by enemy AI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Perception")
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;

	//=========================================================
	// Functions
	//=========================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/**  Called for crouch input */
	void HandleCrouchToggle();
	void HandleCrouchHold();
	void HandleUncrouchHold();

	/**  Called for sprint input */
	void HandleSprintToggle();
	void HandleSprintHold();
	void HandleStopSprintHold();

	/**  Called for opening player inventory - Toggle Inventory Action */
	void ToggleInventory();

	// Player interaction functions
	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();

private:
	FVector LastCheckedPosition;
};
