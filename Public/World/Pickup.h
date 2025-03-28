#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

class UItemBase;

UCLASS()
class STEALTHPROJECT_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//=========================================================
	// Functions
	//=========================================================

	// Sets default values for this actor's properties
	APickup();

	// Called on begin play
	void InitializePickup(const int32 Quantity);

	// During gameplay, when an item is dropped
	void InitializeDrop(UItemBase* Item, const int32 Quantity);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override {};
	virtual void EndInteract() override {};

protected:
	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(VisibleAnywhere, Category="Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Item Database")
	FInteractableData InstanceInteractableData;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Database")
	FDataTableRowHandle ItemRowHandle;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	//=========================================================
	// Functions
	//=========================================================

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact(AStealthProjectCharacter* PlayerCharacter) override;
	void UpdateInteractableData();

	void TakePickup(const AStealthProjectCharacter* Player);

#if WITH_EDITOR
	// Override for editor visuals
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
