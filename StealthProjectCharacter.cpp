// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthProjectCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "Components/LightComponent.h"
#include "Engine/PointLight.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"
#include "Perception/AISense_Sight.h"
#include "UserInterface/HUD/PlayerHUD.h"
#include "UserInterface/Inventory/Component/InventoryComponent.h"
#include "World/Pickup.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AStealthProjectCharacter

AStealthProjectCharacter::AStealthProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Enable crouching in movement component
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false; // set this to true once an actual mesh is obtained
	Mesh1P->CastShadow = false; // set this to true once an actual mesh is obtained
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	InteractionCheckFrequency = 0.1f;
	InteractDistance = 225.0f;

	// Create Inventory component
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);

	// Making sure player is registered for stimuli
	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));
	PerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionStimuliSource->bAutoRegister = true;

	// Stealth variable initialization
	Visibility = 0.f;
	
	LastCheckedPosition = FVector::ZeroVector;
}

void AStealthProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	HUD = Cast<APlayerHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (!HUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *HUD->GetName());
	}

	if (PerceptionStimuliSource)
	{
		PerceptionStimuliSource->RegisterWithPerceptionSystem();
	}
}

void AStealthProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

	// Check if the actor moved and update visibility
	if (FVector::Dist(LastCheckedPosition, GetActorLocation()) > 50.0f)
	{
		CalculateVisibility();
		LastCheckedPosition = GetActorLocation();
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AStealthProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStealthProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AStealthProjectCharacter::Look);

		// Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AStealthProjectCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AStealthProjectCharacter::EndInteract);

		// Menu Toggle
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AStealthProjectCharacter::ToggleInventory);

		// Crouch
		if (ToggleCrouch)
		{
			// If we just want to press once
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AStealthProjectCharacter::HandleCrouchToggle);
		}
		else
		{
			// If we want to hold to crouch
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AStealthProjectCharacter::HandleCrouchHold);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AStealthProjectCharacter::HandleUncrouchHold);
		}

		// Sprint
		if (ToggleSprint)
		{
			// If we just want to press once
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AStealthProjectCharacter::HandleSprintToggle);
		}
		else
		{
			// If we want to hold to crouch
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AStealthProjectCharacter::HandleSprintHold);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AStealthProjectCharacter::HandleStopSprintHold);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AStealthProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AStealthProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AStealthProjectCharacter::HandleCrouchToggle()
{
	if (IsCrouching)
	{
		UnCrouch();
		IsCrouching = false;
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		Crouch();
		IsCrouching = true;
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	}
}

void AStealthProjectCharacter::HandleCrouchHold()
{
	Crouch();
	IsCrouching = true;
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
}

void AStealthProjectCharacter::HandleUncrouchHold()
{
	UnCrouch();
	IsCrouching = false;
	IsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AStealthProjectCharacter::HandleSprintToggle()
{
	if (IsSprinting)
	{
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else if (!IsSprinting && !IsCrouching)
	{
		// We should not be able to sprint if we are crouching
		IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AStealthProjectCharacter::HandleSprintHold()
{
	// We should not be able to sprint if we are crouching
	if (!IsCrouching)
	{
		IsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AStealthProjectCharacter::HandleStopSprintHold()
{
	if (!IsCrouching)
	{
		// This shouldn't enter if we are currently crouching
		IsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AStealthProjectCharacter::ToggleInventory()
{
	HUD->ToggleMenu();
}

void AStealthProjectCharacter::DropItem(UItemBase* Item, const int32 Quantity)
{
	if (!PlayerInventory)
	{
		return;
	}

	// Check if the item is in the inventory
	if (PlayerInventory->FindMatchingItem(Item))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Spawn the item slightly in front of the player
		const FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 50.0f);
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(Item, Quantity);

		// Spawn the actor in the world
		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(Item, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was null/not in inventory."))
	}
}

void AStealthProjectCharacter::CalculateVisibility()
{
	// Reset visibility score so we are getting appropriate values
	// TODO: This should be affected by weather and time of day
	Visibility = 0.f;

	// Max distance for which we want lights to affect visibility
	constexpr float MaxDistance = 1000.f; // We may want to update this value to be higher

	// Grabbing all point lights in the scene
	// TODO: Change this so that we only do this on the level load (so it's only done once)
	TArray<AActor*> PointLights;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APointLight::StaticClass(), PointLights);

	float ClosestDistance = MaxDistance;
	float ClosestLightEffect = 0.f;

	for (AActor* Light : PointLights)
	{
		const APointLight* PointLight = Cast<APointLight>(Light);
		if (const ULightComponent* LightComponent = PointLight->GetLightComponent())
		{
			// Find distance between player and light
			float Distance = FVector::Dist(PointLight->GetActorLocation(), GetActorLocation());

			// Ignore lights outside of max distance
			if (Distance > MaxDistance)
			{
				continue;
			}

			// Calculate how much the light contributes based on distance
			float DistanceEffect = FMath::Clamp(1.0f - (Distance / MaxDistance), 0.0f, 1.0f);

			// We only want to consider the closest, strongest light source
			float LightEffect = DistanceEffect * LightComponent->Intensity;

			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestLightEffect = LightEffect;
			}
		}
	}

	// Normalize visibility score based on intensity range
	Visibility += FMath::Clamp(ClosestLightEffect, 0.0f, 100.0f);
	//UE_LOG(LogTemplateCharacter, Warning, TEXT("Visibility is: %f"), Visibility);
}

void AStealthProjectCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation() };
	FVector TraceEnd{ TraceStart + GetViewRotation().Vector() * InteractDistance };

	FHitResult TraceHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	// Check if we are hitting an item that is interactable
	if (GetWorld()->SweepSingleByChannel(TraceHit, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(50.0f), QueryParams))
	{
		// Check if interface is implemented
		if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				FoundInteractable(TraceHit.GetActor());
				return;
			}

			if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
			{
				return;
			}
		}
	}

	NoInteractableFound();
}

void AStealthProjectCharacter::FoundInteractable(AActor* NewInteractable)
{
	// If we are in the process of interacting, finish the interaction
	if (IsInteracting())
	{
		EndInteract();
	}

	// Check if we are currently in front of an interactable
	if (InteractionData.CurrentInteractable)
	{
		// End the focus since there is a new interactable found
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void AStealthProjectCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		// Hide interaction widget
		HUD->HideInteractionWidget();

		// Interactable is now gone
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void AStealthProjectCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AStealthProjectCharacter::BeginInteract()
{
	// Make sure that interactable state hasn't changed
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			// If the interaction duration is near 0, then interact, otherwise, begin the timer
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction, this, &AStealthProjectCharacter::Interact, TargetInteractable->InteractableData.InteractionDuration, false);
			}
		}
	}
}

void AStealthProjectCharacter::EndInteract()
{
	// Clear the timer handle
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AStealthProjectCharacter::Interact()
{
	// Clear the timer handle
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}
