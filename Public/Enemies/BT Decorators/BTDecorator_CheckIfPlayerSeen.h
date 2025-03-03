// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckIfPlayerSeen.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UBTDecorator_CheckIfPlayerSeen : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckIfPlayerSeen();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;	
};
