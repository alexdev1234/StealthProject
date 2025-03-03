// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "PatrolEnemy.generated.h"

class UAIPerceptionStimuliSourceComponent;
class APatrolEnemyAIController;

UCLASS()
class STEALTHPROJECT_API APatrolEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	//========================================================
	// Functions
	//========================================================
	
	// Sets default values for this character's properties
	APatrolEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* EnemyMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol Group")
	FGameplayTagContainer PatrolGroup;
	
protected:
	//========================================================
	// Functions
	//========================================================
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSource;
};
