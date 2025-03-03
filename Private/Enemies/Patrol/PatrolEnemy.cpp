// ©2024 - Happy Spirits Studios


#include "Enemies/Patrol/PatrolEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Enemies/Patrol/PatrolEnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
APatrolEnemy::APatrolEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the AI controller
	AIControllerClass = APatrolEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	// Set collision capsule size
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	// Set mesh
	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	EnemyMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	EnemyMesh->SetupAttachment(GetCapsuleComponent());

	// Register character for stimuli and sight
	PerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuli"));
	PerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionStimuliSource->bAutoRegister = true;
}

// Called when the game starts or when spawned
void APatrolEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APatrolEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

