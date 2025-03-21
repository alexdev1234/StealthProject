// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsPlayerSeen.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHPROJECT_API UBTDecorator_IsPlayerSeen : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsPlayerSeen();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;	
};


