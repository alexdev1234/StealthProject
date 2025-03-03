// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherController.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	Morning,
	Afternoon,
	Evening,
	Night
};

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
	Sunny,
	Cloudy,
	Rainy,
	Stormy
};

UCLASS()
class STEALTHPROJECT_API AWeatherController : public AActor
{
	GENERATED_BODY()
	
public:
	//========================================================
	// Functions
	//========================================================
	
	// Sets default values for this actor's properties
	AWeatherController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void ChangeWeather(EWeatherType NewWeather);

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void ChangeTimeOfDay(ETimeOfDay NewTimeOfDay);

	//=========================================================
	// Properties and Variables
	//=========================================================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	EWeatherType CurrentWeather = EWeatherType::Sunny;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
	ETimeOfDay CurrentTimeOfDay = ETimeOfDay::Morning;


	//-------- Sunny Weather --------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Sunny Weather")
	float SunnySkyLightIntensity = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Sunny Weather")
	float SunnyDirectionalLightIntensity = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Sunny Weather")
	FLinearColor SunnyLightColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Sunny Weather")
	float SunnyFogIntensity = 0.02f;

	//-------- Cloudy Weather --------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Cloudy Weather")
	float CloudySkyLightIntensity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Cloudy Weather")
	float CloudyDirectionalLightIntensity = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Cloudy Weather")
	FLinearColor CloudyLightColor = FLinearColor(0.7f, 0.7f, 0.7f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Cloudy Weather")
	float CloudyFogIntensity = 0.1f;

	//-------- Rainy Weather --------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Rainy Weather")
	float RainySkyLightIntensity = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Rainy Weather")
	float RainyDirectionalLightIntensity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Rainy Weather")
	FLinearColor RainyLightColor = FLinearColor(0.4f, 0.4f, 0.4f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Rainy Weather")
	float RainyFogIntensity = 0.3f;

	//-------- Stormy Weather --------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Stormy Weather")
	float StormySkyLightIntensity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Stormy Weather")
	float StormyDirectionalLightIntensity = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Stormy Weather")
	FLinearColor StormyLightColor = FLinearColor(0.3f, 0.3f, 0.4f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Stormy Weather")
	float StormyFogIntensity = 0.8f;

	//-------- Niagara Effects --------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather | Effects")
	UNiagaraSystem* RainEffect;
	
protected:
	//========================================================
	// Functions
	//========================================================
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//=========================================================
	// Properties and Variables
	//=========================================================
	FTimerHandle LightningTimerHandle;
	
private:
	//========================================================
	// Functions
	//========================================================
	void UpdateWeather();
	void MakeSunny();
	void MakeCloudy();
	void MakeRainy();
	void MakeStormy();

	void UpdateTimeOfDay();
	void MakeMorning();
	void MakeAfternoon();
	void MakeEvening();
	void MakeNight();

	void StartLightningEffect();
	void StopLightningEffect();
	
	UFUNCTION()
	void TriggerLightning();

	void StartRain();
	void StopRain();

	//=========================================================
	// Properties and Variables
	//=========================================================
	UNiagaraComponent* ActiveRainEffect;
};


