// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PatrolEnemyAIController.generated.h"

class APatrolPoint;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API APatrolEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	APatrolEnemyAIController();

	//========================================================
	// Functions
	//========================================================
	virtual void BeginPlay() override;
	
	void SetPatrolPointInBlackboard();

	bool IsPlayerVisible() const;
	
protected:
	//========================================================
	// Functions
	//========================================================
	void OnPossess(APawn* InPawn) override;
	
	void FindPatrolPointsByTag();

	UFUNCTION()
	void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus);

	//=========================================================
	// Properties and Variables
	//=========================================================
	//APawn* PlayerPawn; // Need to know where player is for perception

	// List of patrol points
	TArray<APatrolPoint*> PatrolPoints;
	int32 CurrentPatrolPointIndex;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	// Perception settings
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SightRadius = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PeripheralVisionAngle = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxAge = 5.f;
};
