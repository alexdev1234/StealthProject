// ©2024 - Happy Spirits Studios


#include "Enemies/Patrol/PatrolEnemyAIController.h"
#include "GameplayTagContainer.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemies/Patrol/PatrolEnemy.h"
#include "StealthProject/StealthProjectCharacter.h"
#include "World/PatrolPoints/PatrolPoint.h"
#include "World/PatrolPoints/PatrolPointManager.h"

APatrolEnemyAIController::APatrolEnemyAIController() :
	PatrolPoints()
{
	// Initialize blackboard component
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	//UE_LOG(LogTemp, Warning, TEXT("Blackboard comp initialized"));

	// Initialize perception
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SetPerceptionComponent(*PerceptionComp);

	// Sight config
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = SightRadius + 100.f; // Should be slightly larger than sight radius
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngle;
	SightConfig->SetMaxAge(MaxAge);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Bind sight to perception
	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(UAISense_Sight::StaticClass());
	PerceptionComp->RegisterComponent();
}

void APatrolEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Enable perception system
	if (PerceptionComp)
	{
		// Bind perception event
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &APatrolEnemyAIController::OnTargetPerceived);

		// Register and enable the perception
		UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), GetPawn());
		PerceptionComp->SetComponentTickEnabled(true);
	}

	if (!PerceptionComp->IsRegistered())
	{
		UE_LOG(LogTemp, Warning, TEXT("Perception is not registered"));
	}
	
	// Get the patrol points for the AI
	FindPatrolPointsByTag();
	CurrentPatrolPointIndex = 0; // Start at the first index
	
	// Make sure we have a valid behavior tree
	if (BehaviorTree && UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp))
	{
		RunBehaviorTree(BehaviorTree);
		//UE_LOG(LogTemp, Warning, TEXT("Started behavior tree"));

		const FVector Location = PatrolPoints[0]->GetActorLocation();
		BlackboardComp->SetValueAsVector("PatrolPointLocation", Location);
	}
}

void APatrolEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), GetPawn());
	}
}

void APatrolEnemyAIController::SetPatrolPointInBlackboard()
{
	if (PatrolPoints.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Patrol points is empty"));
		return;
	}

	// Get current index and loop back if we reach the end
	CurrentPatrolPointIndex = (CurrentPatrolPointIndex + 1) % PatrolPoints.Num();
	//UE_LOG(LogTemp, Warning, TEXT("Current patrol index is: %d"), CurrentPatrolPointIndex);

	if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		const FVector Location = PatrolPoints[CurrentPatrolPointIndex]->GetActorLocation();
		//UE_LOG(LogTemp, Warning, TEXT("Next patrol point: %s"), *Location.ToString());

		FNavLocation NavLocation;
		if (NavSystem->ProjectPointToNavigation(Location, NavLocation))
		{
			if (BlackboardComp)
			{
				BlackboardComp->SetValueAsVector("PatrolPointLocation", Location);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Blackboard comp not set."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get navigation location."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid patrol point"));
	}
}

bool APatrolEnemyAIController::IsPlayerVisible() const
{
	if (PerceptionComp)
	{
		TArray<AActor*> PercievedActors;
		PerceptionComp->GetCurrentlyPerceivedActors(nullptr, PercievedActors);
		for (AActor* Actor : PercievedActors)
		{
			// Check if we see the player
			if (Actor->IsA(AStealthProjectCharacter::StaticClass()))
			{
				return true;
			}
		}
	}
	
	return false;
}

void APatrolEnemyAIController::FindPatrolPointsByTag()
{
	// Clear existing points
	PatrolPoints.Empty();

	if (const APatrolEnemy* PatrolEnemy = Cast<APatrolEnemy>(GetPawn()))
	{
		// Get patrol tag from enemy
		const FGameplayTag PatrolGroupTag = PatrolEnemy->PatrolGroup.First();
		
		// Get all points of a specific tag
		if (const UWorld* World = GetWorld())
		{
			if (const UPatrolPointManager* Manager = World->GetSubsystem<UPatrolPointManager>())
			{
				PatrolPoints = Manager->GetPatrolPointsByTag(PatrolGroupTag);

				//UE_LOG(LogTemp, Warning, TEXT("Found %d patrol points for tag: %s"), PatrolPoints.Num(), *PatrolEnemy->PatrolGroup.First().ToString());
			}
		}
	}

}

void APatrolEnemyAIController::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor && Stimulus.WasSuccessfullySensed())
	{
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsObject("TargetActor", Actor);
			BlackboardComp->SetValueAsBool("HasSeenPlayer", true);
		}
	}
	else
	{
		BlackboardComp->ClearValue("TargetActor");
		BlackboardComp->SetValueAsBool("HasSeenPlayer", false);
	}
}

