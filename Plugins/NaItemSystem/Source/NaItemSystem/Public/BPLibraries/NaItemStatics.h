#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaItemContainer.h"
#include "NaItemStatics.generated.h"


/**
* This is the blueprint function interface for
* general item functions.
* For database-related functions, see NaItemDataStatics.h.
*/

struct FNaItemDescriptor;
struct FNaItemType;
struct FTableRowBase;
struct FNaItemEntry;

UCLASS()
class /*NAITEMSYSTEM_API*/ UNaItemStatics :public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	/*** NaItemDescriptor ***/

	// Check if the input item descriptors absolutely identity.
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal(NaItem Descriptor)", CompactNodeTitle = "==", Keywords = "=="), Category = "NaItemSystem|Item")
	static bool IsEqual_ItemDescriptor(const FNaItemDescriptor & A, const FNaItemDescriptor & B);
	
	/*** NaItemEntry ***/

	// Check if an item entry is valid (correct).
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|Item")
	static bool IsValidItemEntry(const FNaItemEntry & ItemEntry);


	/*** NaItemContainer ***/

	/*--- Get functions ---*/

	// Get max supported item container size. To set this value, reset the macro ITEM_CONTAINER_MAX_SIZE in NaItemContainer.h file.
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|ItemContainer")
	static int GetItemContainerSizeLimit() { return ITEM_CONTAINER_MAX_SIZE; };

	// Get the current container size.
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|ItemContainer")
	static int GetContainerSize(const FNaItemContainer & Target) { return Target.GetSize(); };

	// Check if an index is in the container size. Note: container indeces start from 0 just like arrays.
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|ItemContainer")
	static bool IsInContainerSize(const FNaItemContainer & Target, int Index) { return Target.IsInSize(Index); };

	/**
	* Find item entry from a given index.
	* @Param ReturnType Type of finding result, e.g. found, empty, errors, etc.
	* @Param Result Item entry found. If not found (empty or wrong index), it will be empty!
	*/
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|ItemContainer")
	static void FindEntryFromIndex(const FNaItemContainer & Target, int Position, FNaItemContainerFindingReturn & ReturnType, TSharedPtr<FNaItemEntry> & Result) ;

	// Check if an item entry is valid (not empty).
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|ItemContainer")
	static bool IsValidEntry(TSharedPtr<FNaItemEntry> & Test) { return Test.IsValid(); };

	/*-- Operations --*/

	/**
	* Resize a container.
	* @Param bForce If true, when shrinking, it will force shrinking, deleting the item entries in the shrinked positions.
	* @ReturnValue Whether resizing succeeded.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static bool ResizeContainer(TSharedPtr<FNaItemContainer> Target, int NewSize, bool bForce);

	/**
	* Add item entry to container.
	* @Param bForce If true, it will replace the existing entry if position to add is occupied. Or it will fail.
	* @ReturnValue Whether adding succeeded.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static bool AddItemEntry(TSharedPtr<FNaItemContainer> Target, int Position, const FNaItemEntry & Entry, bool bForce = false);

	/**
	* Add item entry or stack onto the same entry.
	* If the position is empty, add into it; if the position is occupied with absolutely same item (checked from NaItemDescriptor identity), stack on it.
	* @ReturnValue Amount of items remained not added.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static int AddOrStackItemEntry(TSharedPtr<FNaItemContainer> Target, int Position, const FNaItemEntry & Entry);

	/**
	* Remove an entry of given position in a container.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	void RemoveItemEntry(TSharedPtr<FNaItemContainer> Target, int Position);

	/**
	* Move an item entry to another position.
	* @Param bForce If true,
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	bool MoveItemEntry(TSharedPtr<FNaItemContainer> Target, int From, int To, bool bForce = false);

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	/* Swap two entries. This action will not fail if no Out Of Size. */
	void SwapItemEntry(TSharedPtr<FNaItemContainer> Target, int P1, int P2);



};