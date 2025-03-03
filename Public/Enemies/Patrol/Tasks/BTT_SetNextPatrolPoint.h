// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetNextPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UBTT_SetNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetNextPatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};


