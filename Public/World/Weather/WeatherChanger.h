// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherController.h"
#include "WeatherChanger.generated.h"

class UBoxComponent;

UCLASS()
class STEALTHPROJECT_API AWeatherChanger : public AActor
{
	GENERATED_BODY()
	
public:
	//========================================================
	// Functions
	//========================================================
	
	// Sets default values for this actor's properties
	AWeatherChanger();

	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather")
	AWeatherController* WeatherController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather | Weather Type")
	EWeatherType WeatherType = EWeatherType::Sunny;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weather | Time Of Day")
	ETimeOfDay TimeOfDay = ETimeOfDay::Morning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collider | Bounds")
	FVector BoxBounds = FVector(1000.0f, 200.0f, 500.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collider | Bounds")
	float LineThickness = 10.f;
	
protected:
	//========================================================
	// Functions
	//========================================================

	UFUNCTION()
	void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					  bool bFromSweep, const FHitResult& SweepResult);

	//=========================================================
	// Properties and Variables
	//=========================================================
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collider")
	UBoxComponent* BoxCollider;
};
