// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()
	

public:
	//========================================================
	// Functions
	//========================================================
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//=========================================================
	// Properties and Variables
	//=========================================================

protected:
	//========================================================
	// Functions
	//========================================================

	//=========================================================
	// Properties and Variables
	//=========================================================
	APawn* PlayerPawn;
};
