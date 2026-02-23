#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaItemContainer.h"
#include "NaItemStack.h"
#include "NaItemStatics.generated.h"


/**
* This is the blueprint function interface for
* general item functions.
* For database-related functions, see NaItemDataStatics.h.
*/

class UNaGameModeItemSystemComponent;

UCLASS()
class NAITEMSYSTEM_API UNaItemStatics :public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

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
	* Find item stack from a given index.
	* @Param ReturnType Type of finding result, e.g. found, empty, errors, etc.
	* @Param Result Item stack found. If not found (empty or wrong index), it will be null.
	*/
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|ItemContainer")
	static void FindEntryFromIndex(const FNaItemContainer & Target, int Position, ENaItemContainerFindingResult & ReturnType, UNaItemStack*& Result);

	/*-- Operations --*/

	/**
	* Resize a container.
	* @Param bForce If true, when shrinking, it will force shrinking, deleting the item entries in the shrinked positions.
	* @ReturnValue Whether resizing succeeded.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static bool ResizeContainer(UPARAM(ref) FNaItemContainer & Target, int NewSize, bool bForce);

	/**
	* Add item stack to container.
	* @Param bForce If true, it will replace the existing entry if position to add is occupied. Or it will fail.
	* @ReturnValue Whether adding succeeded.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static bool AddItemEntry(UPARAM(ref) FNaItemContainer & Target, int Position, UNaItemStack* Stack, bool bForce = false);

	/**
	* Add item stack or stack onto the same entry.
	* If the position is empty, add into it; if the position is occupied with the same item type, stack on it.
	* Warning: If the target container is not valid, return -1.
	* @ReturnValue Amount of items remained not added.
	*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")
	static int AddOrStackItemEntry(UPARAM(ref) FNaItemContainer & Target, int Position, UNaItemStack* Stack);

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

	/*- Item System game mode component related -*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|GameMode")
	static UNaGameModeItemSystemComponent * GetGameModeItemSystemComponent(UObject* WorldContext);



	/*-- Item usage --*/
//	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|Usage")
//	static 


};