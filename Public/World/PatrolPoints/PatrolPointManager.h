// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "PatrolPointManager.generated.h"

class APatrolPoint;

UCLASS()
class STEALTHPROJECT_API UPatrolPointManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterPatrolPoint(APatrolPoint* PatrolPoint, const FGameplayTag& Group);
	TArray<APatrolPoint*> GetPatrolPointsByTag(const FGameplayTag& Group) const;

private:
	TMap<FGameplayTag, TArray<APatrolPoint*>> PatrolPoints;
};
