// ©2024 - Happy Spirits Studios


#include "Enemies/BaseEnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void ABaseEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Get player reference
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABaseEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn)
	{
		MoveToActor(PlayerPawn, 200.0f);
	}
}
