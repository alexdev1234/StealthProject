// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UCLASS()
class STEALTHPROJECT_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	//========================================================
	// Functions
	//========================================================

	// Sets default values for this character's properties
	ABaseEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//=========================================================
	// Properties and Variables
	//=========================================================

	/** Pawn mesh */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* EnemyMesh;

protected:
	//========================================================
	// Functions
	//========================================================

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	//=========================================================
	// Properties and Variables
	//=========================================================
};
