// ©2024 - Happy Spirits Studios


#include "World/Weather/WeatherController.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/LightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/SkyLight.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AWeatherController::AWeatherController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeatherController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeatherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AWeatherController::ChangeWeather(EWeatherType NewWeather)
{
	CurrentWeather = NewWeather;
	UpdateWeather();
}

void AWeatherController::ChangeTimeOfDay(ETimeOfDay NewTimeOfDay)
{
	CurrentTimeOfDay = NewTimeOfDay;
	UpdateTimeOfDay();
}

void AWeatherController::UpdateWeather()
{
	switch (CurrentWeather)
	{
	case EWeatherType::Sunny:
		MakeSunny();
		break;

	case EWeatherType::Cloudy:
		MakeCloudy();
		break;

	case EWeatherType::Rainy:
		MakeRainy();
		break;

	case EWeatherType::Stormy:
		MakeStormy();
		break;
	}
}

void AWeatherController::UpdateTimeOfDay()
{
	switch (CurrentTimeOfDay)
	{
	case ETimeOfDay::Morning:
		MakeMorning();
		break;

	case ETimeOfDay::Afternoon:
		MakeAfternoon();
		break;

	case ETimeOfDay::Evening:
		MakeEvening();
		break;

	case ETimeOfDay::Night:
		MakeNight();
		break;
	}
}

void AWeatherController::MakeMorning()
{
	UE_LOG(LogTemp, Warning, TEXT("Morning"));

	// Adjust skylight intensity
	ASkyLight* SkyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(1.5f);
	}

	// Adjust directional light rotation
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		DirectionalLight->SetActorRotation(FRotator(-20.f, 90.f, 0.f));
		DirectionalLight->GetLightComponent()->SetLightColor(FLinearColor::White);
		DirectionalLight->GetLightComponent()->SetIntensity(10.0f);
	}
}

void AWeatherController::MakeAfternoon()
{
	UE_LOG(LogTemp, Warning, TEXT("Afternoon"));

	// Adjust skylight intensity
	ASkyLight* SkyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(1.5f);
	}

	// Adjust directional light rotation
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		DirectionalLight->SetActorRotation(FRotator(-90.f, 180.0f, 0.f));
		DirectionalLight->GetLightComponent()->SetLightColor(FLinearColor::White);
		DirectionalLight->GetLightComponent()->SetIntensity(10.0f);
	}
}

void AWeatherController::MakeEvening()
{
	UE_LOG(LogTemp, Warning, TEXT("Evening"));

	// Adjust skylight intensity
	ASkyLight* SkyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(1.5f);
	}

	// Adjust directional light rotation
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		DirectionalLight->SetActorRotation(FRotator(-20.f, 270.f, 0.f));
		DirectionalLight->GetLightComponent()->SetLightColor(FLinearColor::White);
		DirectionalLight->GetLightComponent()->SetIntensity(10.0f);
	}
}

void AWeatherController::MakeNight()
{
	UE_LOG(LogTemp, Warning, TEXT("Night"));

	// Adjust skylight intensity
	ASkyLight* SkyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(1.5f);
	}

	// Adjust directional light rotation
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		// Directional light is about the same position as evening, but significantly less bright and different color.
		DirectionalLight->SetActorRotation(FRotator(-90.f, 0.f, 0.f));
		DirectionalLight->GetLightComponent()->SetLightColor(FLinearColor(0.0f, 0.27f, 0.789f));
		DirectionalLight->GetLightComponent()->SetIntensity(1.0f);
	}
}

void AWeatherController::MakeSunny()
{
	UE_LOG(LogTemp, Warning, TEXT("Sunny"));
	
	// Access skylight and adjust intensity
	ASkyLight* SkyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(SunnySkyLightIntensity);
		UE_LOG(LogTemp, Warning, TEXT("Sky Light Intensity: %f"), SkyLight->GetLightComponent()->Intensity);
	}

	// Access directional light and adjust intensity
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		DirectionalLight->GetLightComponent()->SetIntensity(SunnyDirectionalLightIntensity);
		DirectionalLight->GetLightComponent()->SetLightColor(FLinearColor::White);
		UE_LOG(LogTemp, Warning, TEXT("Directional Light Intensity: %f"), DirectionalLight->GetLightComponent()->Intensity);
	}

	AExponentialHeightFog* HeightFog = Cast<AExponentialHeightFog>(UGameplayStatics::GetActorOfClass(GetWorld(), AExponentialHeightFog::StaticClass()));
	if (HeightFog)
	{
		HeightFog->GetComponent()->SetFogDensity(SunnyFogIntensity); // Somewhat thicker fog for rain
	}

	// Stop rain
	StopRain();
	
	// Stop any lightning effect happening
	StopLightningEffect();
}

void AWeatherController::MakeCloudy()
{
	UE_LOG(LogTemp, Warning, TEXT("Cloudy"));
	// Access skylight and adjust intensity
	ASkyLight* SkyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(CloudySkyLightIntensity);
		UE_LOG(LogTemp, Warning, TEXT("Sky Light Intensity: %f"), SkyLight->GetLightComponent()->Intensity);
	}

	// Access directional light and adjust intensity
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		DirectionalLight->GetLightComponent()->SetIntensity(CloudyDirectionalLightIntensity);
		DirectionalLight->GetLightComponent()->SetLightColor(CloudyLightColor);
		UE_LOG(LogTemp, Warning, TEXT("Directional Light Intensity: %f"), DirectionalLight->GetLightComponent()->Intensity);
	}

	AExponentialHeightFog* HeightFog = Cast<AExponentialHeightFog>(UGameplayStatics::GetActorOfClass(GetWorld(), AExponentialHeightFog::StaticClass()));
	if (HeightFog)
	{
		HeightFog->GetComponent()->SetFogDensity(CloudyFogIntensity); // Light fog
	}

	// Stop rain
	StopRain();
	
	// Stop any lightning effect happening
	StopLightningEffect();
}

void AWeatherController::MakeRainy()
{
	UE_LOG(LogTemp, Warning, TEXT("Rainy"));
	// Access skylight and adjust intensity
	ASkyLight* SkyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(RainySkyLightIntensity);
		UE_LOG(LogTemp, Warning, TEXT("Sky Light Intensity: %f"), SkyLight->GetLightComponent()->Intensity);
	}

	// Access directional light and adjust intensity
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		DirectionalLight->GetLightComponent()->SetIntensity(RainyDirectionalLightIntensity);
		DirectionalLight->GetLightComponent()->SetLightColor(RainyLightColor);
		UE_LOG(LogTemp, Warning, TEXT("Directional Light Intensity: %f"), DirectionalLight->GetLightComponent()->Intensity);
	}

	AExponentialHeightFog* HeightFog = Cast<AExponentialHeightFog>(UGameplayStatics::GetActorOfClass(GetWorld(), AExponentialHeightFog::StaticClass()));
	if (HeightFog)
	{
		HeightFog->GetComponent()->SetFogDensity(RainyFogIntensity); // Somewhat thicker fog for rain
	}
	
	// Play rain
	StartRain();

	// Stop any lightning effect happening
	StopLightningEffect();
}

void AWeatherController::MakeStormy()
{
	UE_LOG(LogTemp, Warning, TEXT("Stormy"));
	
	// Access skylight and adjust intensity
	ASkyLight* SkyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
	if (SkyLight)
	{
		SkyLight->GetLightComponent()->SetIntensity(StormySkyLightIntensity);
		UE_LOG(LogTemp, Warning, TEXT("Sky Light Intensity: %f"), SkyLight->GetLightComponent()->Intensity);
	}

	// Access directional light and adjust intensity
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		DirectionalLight->GetLightComponent()->SetIntensity(StormyDirectionalLightIntensity);
		DirectionalLight->GetLightComponent()->SetLightColor(StormyLightColor);
		UE_LOG(LogTemp, Warning, TEXT("Directional Light Intensity: %f"), DirectionalLight->GetLightComponent()->Intensity);
	}

	AExponentialHeightFog* HeightFog = Cast<AExponentialHeightFog>(UGameplayStatics::GetActorOfClass(GetWorld(), AExponentialHeightFog::StaticClass()));
	if (HeightFog)
	{
		HeightFog->GetComponent()->SetFogDensity(StormyFogIntensity); // Thick fog for stormy day
	}

	// Play rain
	StartRain();
	
	// Lightning Effect
	StartLightningEffect();
}

void AWeatherController::StartLightningEffect()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(LightningTimerHandle))
	{
		float RandLightningInterval = FMath::RandRange(5.f, 15.f);
		GetWorld()->GetTimerManager().SetTimer(LightningTimerHandle, [this]()
		{
			TriggerLightning();
		}, RandLightningInterval, true);
	}
}

void AWeatherController::StopLightningEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(LightningTimerHandle);
}


void AWeatherController::TriggerLightning()
{
	ADirectionalLight* DirectionalLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ADirectionalLight::StaticClass()));
	if (DirectionalLight)
	{
		// Quick flash of light to simulate lightning
		DirectionalLight->GetLightComponent()->SetIntensity(15.0f);

		// Set timer to return intensity to normal
		FTimerHandle LightningTimer;
		GetWorld()->GetTimerManager().SetTimer(LightningTimer, [this, DirectionalLight]()
		{
			DirectionalLight->GetLightComponent()->SetIntensity(StormyDirectionalLightIntensity);
		}, 0.1f, false);
	}

	// Play a thunder sound after a random delay
	/*float ThunderDelay = FMath::RandRange(1.5f, 5.f);
	FTimerHandle ThunderTimer;
	GetWorld()->GetTimerManager().SetTimer(ThunderTimer, [ThunderDelay]()
	{
		if (ThunderSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ThunderSound);
		}
	}, ThunderDelay, false);*/
}

void AWeatherController::StartRain()
{
	if (!ActiveRainEffect && RainEffect)
	{
		// Have the rain follow around the player
		AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (Player)
		{
			ActiveRainEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
				RainEffect,
				Player->GetRootComponent(),
				NAME_None,
				FVector(0.0f, 0.0f, 5000.0f),
				FRotator::ZeroRotator,
				EAttachLocation::Type::KeepRelativeOffset,
				true);
		}
	}
}

void AWeatherController::StopRain()
{
	if (ActiveRainEffect)
	{
		ActiveRainEffect->DestroyComponent();
		ActiveRainEffect = nullptr;
	}
}
