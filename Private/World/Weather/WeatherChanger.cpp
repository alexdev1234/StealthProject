// ©2024 - Happy Spirits Studios


#include "World/Weather/WeatherChanger.h"
#include "Components/BoxComponent.h"
#include "StealthProject/StealthProjectCharacter.h"

// Sets default values
AWeatherChanger::AWeatherChanger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Box collider
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetupAttachment(RootComponent);
	
	// Size of box collider
	BoxCollider->SetBoxExtent(BoxBounds);
	BoxCollider->SetLineThickness(LineThickness);

	// Bind overlap event
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeatherChanger::OnBoxOverlapBegin);
}

void AWeatherChanger::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// Check if we are the player character
		if (OtherActor->IsA(AStealthProjectCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Changing weather and/or time of day."));
			WeatherController->ChangeWeather(WeatherType);
			WeatherController->ChangeTimeOfDay(TimeOfDay);
		}
	}
}

