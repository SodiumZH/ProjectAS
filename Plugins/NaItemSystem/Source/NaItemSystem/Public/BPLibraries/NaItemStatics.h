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

	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|ItemContainer")



};