// ©2024 - Happy Spirits Studios


#include "World/PatrolPoints/PatrolPoint.h"
#include "Components/BillboardComponent.h"
#include  "World/PatrolPoints/PatrolPointManager.h"

// Sets default values
APatrolPoint::APatrolPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Visual component of patrol point only seen in editor
	UBillboardComponent* billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Mesh"));
	billboard->SetupAttachment(RootComponent);
	
	// Set default sprite
	static ConstructorHelpers::FObjectFinder<UTexture2D> texture(TEXT("/Engine/EditorResources/Goal_WayPoint"));
	if(texture.Succeeded())
	{
		billboard->SetSprite(texture.Object);
	}
}

void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UPatrolPointManager* manager = World->GetSubsystem<UPatrolPointManager>())
		{
			manager->RegisterPatrolPoint(this, PatrolGroup.GetByIndex(0));
		}
	}
}
