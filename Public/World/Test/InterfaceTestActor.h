// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

UCLASS()
class STEALTHPROJECT_API AInterfaceTestActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//=========================================================
	// Functions
	//=========================================================
	AInterfaceTestActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//=========================================================
	// Overrided Interface
	//=========================================================
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(AStealthProjectCharacter* Playercharacter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//=========================================================
	// Variables
	//=========================================================
	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData;
};
