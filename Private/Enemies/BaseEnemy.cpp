// ©2024 - Happy Spirits Studios


#include "Enemies/BaseEnemy.h"
#include "Enemies/BaseEnemyAIController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set collision capsule size
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	// Set mesh
	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	EnemyMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	EnemyMesh->SetupAttachment(GetCapsuleComponent());

	// Set the controller for the enemy
	AIControllerClass = ABaseEnemyAIController::StaticClass();

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

