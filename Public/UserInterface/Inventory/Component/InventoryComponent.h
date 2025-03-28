// ©2024 - Happy Spirits Studios

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// Delegates
DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);

// Forward Declarations
class UItemBase;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	NoItemAdded UMETA(DisplayName = "No item added"),
	PartialAmountItemAdded UMETA(DisplayName = "Partial amount added"),
	AllItemAdded UMETA(DisplayName = "All added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
		AmountAdded(0),
		Result(EItemAddResult::NoItemAdded),
		ResultMessage(FText::GetEmpty())
	{};

	// Amount added to inventory
	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	int32 AmountAdded;

	// Resulting operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult Result;

	// Message that can be passed with the result
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult Result;
		Result.AmountAdded = 0;
		Result.Result = EItemAddResult::NoItemAdded;
		Result.ResultMessage = ErrorText;

		return Result;
	}

	static FItemAddResult AddedPartial(const int32 Amount, const FText& ErrorText)
	{
		FItemAddResult Result;
		Result.AmountAdded = Amount;
		Result.Result = EItemAddResult::PartialAmountItemAdded;
		Result.ResultMessage = ErrorText;

		return Result;
	}

	static FItemAddResult AddedAll(const int32 Amount, const FText& ResultText)
	{
		FItemAddResult Result;
		Result.AmountAdded = Amount;
		Result.Result = EItemAddResult::AllItemAdded;
		Result.ResultMessage = ResultText;

		return Result;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	//=========================================================
	// Properties and Variables
	//=========================================================

	FOnInventoryUpdated OnInventoryUpdated;

	//=========================================================
	// Functions
	//=========================================================

	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UItemBase* Item);

	UFUNCTION(Category = "Inventory")
	UItemBase* FindMatchingItem(UItemBase* Item) const;
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextItemByID(UItemBase* Item) const;
	UFUNCTION(Category = "Inventory")
	UItemBase* FindNextPartialStack(UItemBase* Item) const;

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UItemBase* Item);
	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UItemBase* Item, const int32 amount);
	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UItemBase* Item, const int32 amount);

	// Getters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetInventoryTotalWeight() const { return Weight; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return WeightCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE int32 GetSlotsCapacity() const { return SlotsCapacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return Inventory; };

	// Setters
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetWeightCapacity(float capacity) { WeightCapacity = capacity; };
	UFUNCTION(Category = "Inventory")
	FORCEINLINE void SetSlotsCapacity(int32 capacity) { SlotsCapacity = capacity; };

protected:
	//=========================================================
	// Properties and Variables
	//=========================================================

	UPROPERTY(VisibleAnywhere, Category="Inventory")
	float Weight;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float WeightCapacity;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 SlotsCapacity;

	// A TArray acts similarly to a map
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> Inventory;

	//=========================================================
	// Functions
	//=========================================================

	// Called when the game starts
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UItemBase* Item);
	int32 HandleStackableItems(UItemBase* Item, int32 requestedAmount);
	int32 CalculateWeightAmount(UItemBase* Item, int32 requestedAmount);
	int32 CalculateNumberForFullStacks(UItemBase* Item, int32 requestedAmount);

	void AddNewItem(UItemBase* Item, const int32 amount);
};
