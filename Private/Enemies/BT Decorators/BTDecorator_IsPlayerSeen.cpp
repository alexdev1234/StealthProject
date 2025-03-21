// ©2024 - Happy Spirits Studios


#include "Enemies/BT Decorators/BTDecorator_IsPlayerSeen.h"

#include "Enemies/Patrol/PatrolEnemyAIController.h"

UBTDecorator_IsPlayerSeen::UBTDecorator_IsPlayerSeen()
{
	NodeName = "Check if Player is Seen";
}

bool UBTDecorator_IsPlayerSeen::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return false;
	}

	APatrolEnemyAIController* PatrolController = Cast<APatrolEnemyAIController>(Controller);
	if (!PatrolController)
	{
		return false;
	}

	return PatrolController->IsPlayerVisible();
}