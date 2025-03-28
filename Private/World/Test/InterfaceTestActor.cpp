// ©2024 - Happy Spirits Studios


#include "World/Test/InterfaceTestActor.h"

// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableData = InstanceInteractableData;
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::BeginInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("Calling Begin Interact on test actor"));
}

void AInterfaceTestActor::EndInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("Calling End Interact on test actor"));
}

void AInterfaceTestActor::Interact(AStealthProjectCharacter* Playercharacter)
{
	//UE_LOG(LogTemp, Warning, TEXT("Calling Interact on test actor"));
}
