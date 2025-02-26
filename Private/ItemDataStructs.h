// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

/**
 * 
 */

// Enums
UENUM()
enum class EItemQuality
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Objective UMETA(DisplayName = "Primary Objective")
};

UENUM()
enum class EItemType
{
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Consumable UMETA(DisplayName = "Consumable"),
	Throwable UMETA(DisplayName = "Throwable"),
	Valuable UMETA(DisplayName = "Valuable"),
	Quest UMETA(DisplayName = "Quest")
};

// Item Data structures
USTRUCT()
struct FItemStatistics // item stats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float VisibilityValue;

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	float HealValue;

	UPROPERTY(EditAnywhere)
	float DistractionValue;

	UPROPERTY(EditAnywhere)
	float SellValue;
};

USTRUCT()
struct FItemTextData // user interface
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	FText InteractionText;

	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT()
struct FItemNumericData // inventory data
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;

	UPROPERTY(EditAnywhere)
	int32 MaxStacks;

	UPROPERTY(EditAnywhere)
	bool IsStackable;
};

USTRUCT()
struct FItemAssetData // visual data
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStats;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData ItemText;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData ItemNumeric;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData ItemAssetData;
};
