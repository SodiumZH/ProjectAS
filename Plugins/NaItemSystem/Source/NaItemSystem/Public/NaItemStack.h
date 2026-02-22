#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Dom/JsonObject.h"
#include "NaItemType.h"
#include "NaItemStack.generated.h"

/**
 * UNaItemStack - Represents a stack of items with count and custom data
 * Contains:
 * - Reference to item type (singleton defining properties/behavior)
 * - Stack count
 * - Nested custom data tree for item-specific information
 */
UCLASS(BlueprintType)
class NAITEMSYSTEM_API UNaItemStack : public UObject
{
	GENERATED_BODY()

public:
	UNaItemStack();

	/***** Properties *****/

	/** Reference to item type singleton defining this item's properties and behavior */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stack")
	UNaItemType* ItemType;

	/** Number of items in this stack */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stack", meta = (ClampMin = "0", ClampMax = "999"))
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Stack", meta = (ClampMin = "0", ClampMax = "999"))
	TOptional<FText> CustomName;
	/***** Stack Management Functions *****/

	/**
	 * Check if this stack is empty (no type or zero count)
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack")
	bool IsEmpty() const;

	/**
	 * Check if this stack is valid (has type and positive count)
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack")
	bool IsValid() const;

	/**
	 * Get maximum stack size for this item type
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack")
	int32 GetMaxStackSize() const;

	/**
	 * Check if stack is at maximum capacity
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack")
	bool IsFullStack() const;

	/**
	 * Get remaining capacity in this stack
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack")
	int32 GetRemainingCapacity() const;

	/**
	 * Increase stack count
	 * @param Amount Amount to add
	 * @return Actual amount added (clamped by max stack size)
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack")
	int32 Grow(int32 Amount);

	/**
	 * Decrease stack count
	 * @param Amount Amount to remove
	 * @return Actual amount removed
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack")
	int32 Shrink(int32 Amount);

	/**
	 * Set stack count (clamped to valid range)
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack")
	void SetCount(int32 NewCount);

	/**
	 * Clear this stack (set to empty)
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack")
	void Clear();

	/***** Stacking Functions *****/

	/**
	 * Check if this stack can merge with another stack
	 * Checks if item types match and custom data is compatible
	 * @param Other The other stack to check
	 * @param bCheckCustomData If true, custom data must match exactly
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack")
	bool CanStackWith(const UNaItemStack* Other, bool bCheckCustomData = true) const;

	/**
	 * Try to merge another stack into this stack
	 * @param Other The stack to merge from
	 * @param bConsumeOther If true, shrink the other stack as items are added
	 * @return Amount of items actually merged
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack")
	int32 MergeFrom(UNaItemStack* Other, bool bConsumeOther = true);

	/**
	 * Split this stack into two
	 * @param SplitCount How many to move to new stack
	 * @return New stack containing split items (null if can't split)
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack")
	UNaItemStack* Split(int32 SplitCount);

	/***** Usage Functions *****/

	/**
	 * Use this item stack (calls ItemType->OnUse)
	 * @param User The actor using the item
	 * @param Target Optional target actor
	 * @return Whether item was successfully used
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack")
	bool Use(AActor* User, AActor* Target = nullptr);

	/***** Display Functions *****/

	/**
	 * Get display name (from ItemType or custom name in data)
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack|Display")
	FText GetDisplayName() const;

	/**
	 * Get tooltip text for UI
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack|Display")
	FText GetTooltipText() const;

	/**
	 * Get item icon
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack|Display")
	UTexture2D* GetIcon() const;

	/***** Serialization *****/

	/**
	 * Serialize to FArchive (for save games, network replication, etc.)
	 */
	virtual void Serialize(FArchive& Ar) override;

	/**
	 * Export custom data to JSON string (for debugging/logging)
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack|Custom Data")
	FString ExportCustomDataToString() const;

	/**
	 * Import custom data from JSON string
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack|Custom Data")
	bool ImportCustomDataFromString(const FString& JsonString);

	/***** Static Factory Functions *****/

	/**
	 * Create a new item stack
	 * @param Outer Outer object for GC (usually GetTransientPackage() or a persistent object)
	 * @param Type Item type
	 * @param InCount Stack count
	 * @return New item stack instance
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack", meta = (WorldContext = "WorldContextObject"))
	static UNaItemStack* CreateItemStack(UObject* WorldContextObject, UNaItemType* Type, int32 InCount = 1);

	/**
	 * Create an empty item stack
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack", meta = (WorldContext = "WorldContextObject"))
	static UNaItemStack* CreateEmptyStack(UObject* WorldContextObject);

	/**
	 * Create a deep copy of an existing item stack (including custom data)
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack", meta = (WorldContext = "WorldContextObject"))
	static UNaItemStack* CopyItemStack(UObject* WorldContextObject, const UNaItemStack* Source);

	/***** Comparison *****/

	/**
	 * Check if two stacks are exactly equal (same type, count, and custom data)
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack", meta = (CompactNodeTitle = "=="))
	static bool AreStacksEqual(const UNaItemStack* A, const UNaItemStack* B);

	/**
	 * Check if two stacks have same type and custom data (ignoring count)
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack")
	static bool AreStacksSameTypeAndData(const UNaItemStack* A, const UNaItemStack* B);

	/***** C++ Helpers *****/

	/**
	 * Get internal JSON object for advanced C++ usage
	 * Creates one if it doesn't exist
	 */
	TSharedPtr<FJsonObject> GetOrCreateCustomData();

	/**
	 * Get internal JSON object (const version)
	 * Returns null if doesn't exist
	 */
	TSharedPtr<FJsonObject> GetCustomData() const;

	/**
	 * Set custom data from existing JSON object
	 */
	void SetCustomData(TSharedPtr<FJsonObject> InData);

	/**
	 * Check if custom data object exists and has entries
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack|Custom Data")
	bool HasCustomData() const;

	/**
	 * Check if custom data has a specific key
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack|Custom Data")
	bool HasCustomDataKey(const FString& Key) const;

	/**
	 * Get a string value from custom data
	 */
	UFUNCTION(BlueprintPure, Category = "Item Stack|Custom Data")
	FString GetCustomString(const FString& Key, const FString& DefaultValue = TEXT("")) const;

	/**
	 * Merge custom data from another stack
	 * @param Other Source stack to merge from
	 * @param bOverwrite If true, overwrite existing keys
	 */
	void MergeCustomData(const UNaItemStack* Other, bool bOverwrite = true);

	/**
	 * Consume this item (calls ItemType->OnConsume)
	 * @param User The actor consuming the item
	 * @return Whether item was successfully consumed
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Stack")
	bool Consume(AActor* User);

protected:
	/**
	 * Clamp count to valid range [0, MaxStackSize]
	 */
	void ClampCount();

	/**
	 * Compare two JSON objects for equality
	 */
	static bool AreJsonObjectsEqual(const TSharedPtr<FJsonObject>& A, const TSharedPtr<FJsonObject>& B);

private:
	/**
	 * Internal JSON object storing nested custom data
	 * This is serializable and supports arbitrary nesting
	 */
	TSharedPtr<FJsonObject> CustomData;
};