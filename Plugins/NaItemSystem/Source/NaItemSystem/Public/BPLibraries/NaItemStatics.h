#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
class UNaGameModeItemSystemComponent;

UCLASS()
class NAITEMSYSTEM_API UNaItemStatics :public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:


	/*** Empty struct related **/

	/* Static empty objects since BP doesn't support shared ptr */

	/* A default item descriptor representing empty */
	static const FNaItemDescriptor EmptyItemDescriptor;

	/* A default item descriptor representing empty */
	static const FNaItemEntry EmptyItemEntry;

	/** Check if an item descriptor is NOT empty. 
	* If an entry's item type is zero or invalid, it will be regarded empty.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Not Empty (Item Descriptor)", Keywords = "is empty"), Category = "NaItemSystem|Item")
	static bool NotEmpty_Desc(const FNaItemDescriptor & Test);

	/** Check if an item entry is NOT empty. 
	* If an entry's item type is zero or invalid, it will be regarded empty.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Not Empty (Item Entry)", Keywords = "is empty"), Category = "NaItemSystem|Item")
	static bool NotEmpty_Entry(const FNaItemEntry & Test);

	/* Get a copy of the empty item descriptor */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Empty Item Descriptor", CompactNodeTitle = "0", Keywords = "0 empty"), Category = "NaItemSystem|Item")
	static void GetEmptyItemDescriptor(FNaItemDescriptor & EmptyDescriptor);

	/* Get a copy of the empty item entry */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Empty Item Entry", CompactNodeTitle = "0", Keywords = "0 empty"), Category = "NaItemSystem|Item")
	static void GetEmptyItemEntry(FNaItemEntry & EmptyEntry);


	/*** NaItemDescriptor ***/

	// Check if the input item descriptors absolutely identity.
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal(NaItem Descriptor)", CompactNodeTitle = "==", Keywords = "=="), Category = "NaItemSystem|Item")
	static bool IsEqual_ItemDescriptor(const FNaItemDescriptor & A, const FNaItemDescriptor & B) { return A == B; };
	
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
	static void FindEntryFromIndex(const FNaItemContainer & Target, int Position, ENaItemContainerFindingResult & ReturnType, FNaItemEntry & Result) ;

	/*-- Operations --*/

	/**
	* Resize a container.
	* @Param bForce If true, when shrinking, it will force shrinking, deleting the item entries in the shrinked positions.
	* @ReturnValue Whether resizing succeeded.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static bool ResizeContainer(UPARAM(ref) FNaItemContainer & Target, int NewSize, bool bForce);

	/**
	* Add item entry to container.
	* @Param bForce If true, it will replace the existing entry if position to add is occupied. Or it will fail.
	* @ReturnValue Whether adding succeeded.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static bool AddItemEntry(UPARAM(ref) FNaItemContainer & Target, int Position, const FNaItemEntry & Entry, bool bForce = false);

	/**
	* Add item entry or stack onto the same entry.
	* If the position is empty, add into it; if the position is occupied with absolutely same item (checked from NaItemDescriptor identity), stack on it.
	* This operation is data-dependent, so a world context is needed.
	* Warning: If the target container is not valid, return -1.
	* @ReturnValue Amount of items remained not added.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static int AddOrStackItemEntry(UPARAM(ref) FNaItemContainer & Target, UObject* WorldContext, int Position, const FNaItemEntry & Entry);

	/**
	* Remove an entry of given position in a container.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static void RemoveItemEntry(UPARAM(ref) FNaItemContainer & Target, int Position);

	/**
	* Move an item entry to another position.
	* @Param bForce If true, the target position will be overwritten if occupied.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static bool MoveItemEntry(UPARAM(ref) FNaItemContainer & Target, int From, int To, bool bForce = false);

	/* Swap two entries. This action will not fail if no Out Of Size. */
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static void SwapItemEntry(UPARAM(ref) FNaItemContainer & Target, int P1, int P2);

	/*- Item System game mode component related 0-*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|GameMode")
	static UNaGameModeItemSystemComponent * GetGameModeItemSystemComponent(UObject* WorldContext);
};