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
#include "Kismet/GameplayStatics.h"
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
	Visibility = 50.f;
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

	CalculateVisibility();
	
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
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
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.bNoFail = true;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Spawn the item slightly in front of the player
		const FVector spawnLocation = GetActorLocation() + (GetActorForwardVector() * 50.0f);
		const FTransform spawnTransform(GetActorRotation(), spawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(Item, Quantity);

		// Spawn the actor in the world
		APickup* pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), spawnTransform, spawnParams);

		pickup->InitializeDrop(Item, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was null/not in inventory."))
	}
}

void AStealthProjectCharacter::CalculateVisibility()
{
	// Reset visibility score at start of each calculation
	Visibility = 0.f;
	
	// Max distance at which light affects visibility
	float MaxDistance = 1000.f;

	// Get all point light actors in scene
	TArray<AActor*> PointLights;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APointLight::StaticClass(), PointLights);
	for (AActor* Light : PointLights)
	{
		const APointLight* PointLight = Cast<APointLight>(Light);
		if (const ULightComponent* LightComponent = PointLight->GetLightComponent())
		{
			// Find distance between player and light
			float Distance = FVector::Dist(PointLight->GetActorLocation(), GetActorLocation());

			// Skip any light that is too far away
			if (Distance > MaxDistance)
			{
				continue;
			}

			// Get light intensity
			float LightIntensity = LightComponent->Intensity;

			// Calculate visibility contribution from light
			float DistanceEffect = FMath::Clamp(1.0f - (Distance / MaxDistance), 0.0f, 1.0f);

			// Add light contribution to total visibility score
			Visibility += DistanceEffect * LightIntensity;
		}
	}

	// TODO: Get rid of magic number
	Visibility = FMath::Clamp(Visibility / 50.f, 0.0f, 1.0f);
	
	UE_LOG(LogTemp, Warning, TEXT("Visibility is: %f"), Visibility);
}

void AStealthProjectCharacter::CalculateWeight()
{
	// TODO: Weight calculation based on inventory weight + base player weight
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
