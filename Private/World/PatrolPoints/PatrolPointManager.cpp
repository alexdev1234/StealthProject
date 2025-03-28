// ©2024 - Happy Spirits Studios


#include "World/PatrolPoints/PatrolPointManager.h"
#include "World/PatrolPoints//PatrolPoint.h"

void UPatrolPointManager::RegisterPatrolPoint(APatrolPoint* PatrolPoint, const FGameplayTag& Group)
{
	if (PatrolPoint)
	{
		PatrolPoints.FindOrAdd(Group).Add(PatrolPoint);
	}
}

TArray<APatrolPoint*> UPatrolPointManager::GetPatrolPointsByTag(const FGameplayTag& Group) const
{
	const TArray<APatrolPoint*>* points = PatrolPoints.Find(Group);
	return points ? *points : TArray<APatrolPoint*>();
}
