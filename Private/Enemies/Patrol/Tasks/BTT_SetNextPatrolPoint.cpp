// ©2024 - Happy Spirits Studios


#include "Enemies/Patrol/Tasks/BTT_SetNextPatrolPoint.h"

#include "Enemies/Patrol/PatrolEnemyAIController.h"

UBTT_SetNextPatrolPoint::UBTT_SetNextPatrolPoint()
{
	NodeName = "Set Next Patrol Point";
}

EBTNodeResult::Type UBTT_SetNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	APatrolEnemyAIController* AIController = Cast<APatrolEnemyAIController>(Comp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// Get the next patrol point
	AIController->SetPatrolPointInBlackboard();
	
	return EBTNodeResult::Succeeded;
}