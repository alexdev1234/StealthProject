// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UBTTask_MoveToPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToPatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
};
