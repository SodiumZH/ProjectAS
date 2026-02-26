#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NaItemStack.h"
#include "NaItemInventory.generated.h"

/**
 * UNaItemInventory - UObject-based inventory storing an ordered list of item stacks.
 * Null entries represent empty slots.
 */
UCLASS(BlueprintType, Blueprintable)
class NAITEMSYSTEM_API UNaItemInventory : public UObject
{
	GENERATED_BODY()

public:
	UNaItemInventory();

	/***** Query *****/

	/** Get the inventory size (number of slots) */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	int32 GetSize() const;

	/** Check if a slot index is in range */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	bool IsValidSlot(int32 Slot) const;

	/** Get the stack at a slot (returns nullptr for empty or out-of-range) */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	UNaItemStack* GetSlot(int32 Slot) const;

	/** Check if a slot is empty (null or empty stack) */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	bool IsSlotEmpty(int32 Slot) const;

	/** Find the first empty slot; returns -1 if full */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	int32 FindFirstEmptySlot() const;

	/** Find first slot with the given item type starting from StartFrom; returns -1 if not found */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	int32 FindItemByType(const UNaItemType* Type, int32 StartFrom = 0) const;

	/** Count total items of a given type across all slots */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	int32 CountItemByType(const UNaItemType* Type) const;

	/***** Direct Slot Operations *****/

	/** Directly set a slot. Null or empty stack clears the slot. */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	void SetSlot(int32 Slot, UNaItemStack* Stack);

	/** Clear a slot (set to nullptr) */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	void ClearSlot(int32 Slot);

	/** Swap contents of two slots */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	void SwapSlots(int32 SlotA, int32 SlotB);

	/**
	 * Try to merge the source stack into the dest stack.
	 * Returns true if any items were moved. Clears source slot if it becomes empty.
	 */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	bool MergeSlots(int32 SourceSlot, int32 DestSlot);

	/***** Giving Items *****/

	/**
	 * Give an item stack to the inventory.
	 * Merges into existing stacks first, then fills empty slots.
	 * Returns the number of items that could NOT be added (0 = fully added).
	 * The input Stack's count is reduced by the amount actually added.
	 */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	int32 GiveItem(UNaItemStack* Stack);

	/**
	 * Like GiveItem but only executes if ALL items fit.
	 * Returns true if succeeded. Does not modify the input stack on failure.
	 */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	bool GiveItemComplete(UNaItemStack* Stack);

	/** Check if the given stack can be fully added without modifying anything */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	bool CanGiveItemComplete(const UNaItemStack* Stack) const;

	/***** Consuming / Taking Items *****/

	/**
	 * Remove up to Amount items of the given type from the inventory.
	 * Returns the number actually consumed. Clears slots that become empty.
	 */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	int32 ConsumeItemByType(UNaItemType* Type, int32 Amount);

	/** Check if at least Amount items of the given type exist in the inventory */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Inventory")
	bool CanConsumeItemByType(const UNaItemType* Type, int32 Amount) const;

	/**
	 * Take a given amount from a slot, returning a new stack with those items.
	 * Clears the slot if all items are taken. Returns nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	UNaItemStack* TakeFromSlot(int32 Slot, int32 Amount);

	/***** Resize *****/

	/**
	 * Resize the inventory.
	 * If shrinking and occupied slots would be removed, fails unless bForce is true.
	 * When force-shrinking, items in removed slots are lost.
	 */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	bool Resize(int32 NewSize, bool bForce = false);

	/***** Maintenance *****/

	/** Clear the entire inventory (all slots to nullptr) */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	void ClearAll();

	/** Find any stacks with Count==0 and set their slots to nullptr */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	void ClearEmptyStacks();

	/***** Static Factory *****/

	/** Create a new inventory with the given size. All slots initialized to nullptr. */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Inventory")
	static UNaItemInventory* CreateInventory(UObject* Outer, int32 Size = 64);

protected:
	/** Ordered list of item stacks. Null entries represent empty slots. */
	UPROPERTY()
	TArray<UNaItemStack*> Slots;
};
