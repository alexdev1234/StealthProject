// ©2024 - Happy Spirits Studios


#include "Enemies/Patrol/Tasks/BTTask_MoveToPatrolPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Patrol/PatrolEnemyAIController.h"

UBTTask_MoveToPatrolPoint::UBTTask_MoveToPatrolPoint()
{
	NodeName = "Move To Patrol Point";
}

EBTNodeResult::Type UBTTask_MoveToPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Check if we have seen the player or not. If true, we abort this task and start chasing the player.
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (BlackboardComponent && BlackboardComponent->GetValueAsBool("HasSeenPlayer"))
	{
		return EBTNodeResult::Aborted;
	}

	// Get the AI controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// Get the patrol point and check to see if its invalid
	FVector PatrolPoint = BlackboardComponent->GetValueAsVector("PatrolPointLocation");
	if (PatrolPoint.IsZero())
	{
		return EBTNodeResult::Failed;
	}

	AIController->MoveToLocation(PatrolPoint);

	return EBTNodeResult::InProgress;
}
