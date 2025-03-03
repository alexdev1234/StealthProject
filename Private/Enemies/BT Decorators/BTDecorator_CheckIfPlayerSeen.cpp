// ©2024 - Happy Spirits Studios


#include "Enemies/BT Decorators/BTDecorator_CheckIfPlayerSeen.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UBTDecorator_CheckIfPlayerSeen::UBTDecorator_CheckIfPlayerSeen()
{
	NodeName = "Check If Player Seen";
}

bool UBTDecorator_CheckIfPlayerSeen::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	// Get AI Controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	// Get Blackboard Component
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return false;
	}

	// Check if target actor is valid
	UObject* TargetActor = BlackboardComponent->GetValueAsObject("TargetActor");
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor not found!"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Decorator: TargetActor is %s"), *TargetActor->GetName());
	return true;
}
