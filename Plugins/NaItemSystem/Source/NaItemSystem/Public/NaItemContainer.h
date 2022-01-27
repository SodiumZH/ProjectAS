#pragma once

#include "CoreMinimal.h"
#include "NaItemContainer.generated.h"

// This value is intended to avoid integer overflow.
#define ITEM_CONTAINER_MAX_SIZE 10000

struct FNaItemType;
struct FNaItemDescriptor;
struct FNaItemEntry;
class UNaGameModeItemSystemComponent;


/* Result types for browsing an item container */
UENUM()
enum class ENaItemContainerFindingResult :uint8 {
	ICFR_Filled		UMETA(DisplayName = "Filled"),			// Browsed successfully, and the socket is containing valid item(s).
	ICFR_Empty		UMETA(DisplayName = "Empty"),		// Browsed successfully, and the socket is empty.
	ICFR_OOS		UMETA(DisplayName = "Out of Size"),		// Trying to browse at the position out of the max size
	/* The above values are correct results */
	/* The below values are errors that should not appear */
	ICFR_InvID		UMETA(DisplayName = "Invalid Index"),	// Trying to browse at invalid position (negative or out of ITEM_CONTAINER_MAX_SIZE defined in NaItemManager.h)
	ICFR_InvVal 	UMETA(DisplayName = "Invalid Item Record"),		// Found record correctly, but the item record itself is invalid (containing something wrong)
	ICFR_Error		UMETA(DisplayName = "Error")			// Unexpected error
};


/* Return values for Searching in a container */
struct FNaItemContainerFindingReturn {
	// Values
	TSharedPtr<FNaItemEntry> EntryPtr;
	ENaItemContainerFindingResult Result;

	// Initializer when not found
	enum class NotFoundType :uint8 {
		Empty, OOS, InvID, InvVal, Error
	};

	// Found item successfully
	FNaItemContainerFindingReturn(TSharedPtr<FNaItemEntry> InEntryPtr);

	// Not found
	FNaItemContainerFindingReturn(NotFoundType);

	// Copy
	FNaItemContainerFindingReturn(const FNaItemContainerFindingReturn & Copy) :
		EntryPtr(Copy.EntryPtr),
		Result(Copy.Result)
	{};
};

/* Structure that contains a series of items.
* It can be used to describe a bag, shop, etc.
*/
USTRUCT(Blueprintable)
struct /*NAITEMSYSTEM_API*/ FNaItemContainer {

	GENERATED_BODY()

protected:

	/* Content as an array */
	TArray<TSharedPtr<FNaItemEntry>> Content;

	/* Max size */
	int Size = 64;



// Check if Content.Num() == Size. CRASH when failed.
#define CheckSize() checkf(Content.Num() == Size, TEXT("NaItemContainer Error: Container size != Content array actual size."))


public:

	/***** Constructors *****/

	// Make an empty container. Default size is 64.
	FNaItemContainer();

	// Make empty container with givin size.
	FNaItemContainer(int InSize);

	// Copy from other. 
	FNaItemContainer(const FNaItemContainer & CopyFrom);

	/** Resize the container.
	* @Param bForce If true, when shrinked area contains items, it will ignore them (causing the items lost!). Or it will fail if shrinked area contains items.
	*/
	bool Resize(int NewSize, bool bForce = false);

	/*----------------------------------------------------*/
	/** Get functions **/

	// Get the size.
	FORCEINLINE int GetSize() const { return Size; };

	// Check if a position is in size. Please note that the valid values is [0,Size) just like an array.
	FORCEINLINE bool IsInSize(int Position) const { return Position >= 0 && Position < Size; };

	// Get entry from index
	FNaItemContainerFindingReturn Find(int Index) const;

	/** Get all positions that contains item in given type
	* Return whether there are any.
	* @Param TypeIndex The index in item type datatable of the item type.
	* @Param Positions Return positions containing the item of given type.
	* @Param bIncludeUniquified If true, the search result will include uniquified items.
	*/
	bool FindItem(int TypeIndex, TArray<int>& Positions, bool bIncludeUniquified = false);

	/** Get all positions that is identical to the item, including unique item information and uniquification.
	* Return whether there are any.
	*/
	bool FindItemExplicit(const FNaItemDescriptor & Type, TArray<int>& Positions);


	/** Operations **/

	/** Add entry. 
	* If the container position is occupied, the values will not be added unless bForce == true.
	* Return whether succeeded.
	*/
	bool AddEntry(int Position, const FNaItemEntry & Entry, bool bForce = false);


	/* Remove an item from position. */
	void RemoveEntry(int Position);

	/* Move an item entry to another position.
	* @Param bForce If true, the "from" entry will overwrite the "to" entry if the latter is occupied.
	* @ReturnValue Whether moved successfully. Fail if moving from an empty position or to an occupied position without bForce == true. 
	*/
	bool MoveEntry(int From, int To, bool bForce = false);

	/* Swap two entries. This action will not fail if no Out Of Size. */
	void SwapEntry(int P1, int P2);
	



	/*----- Data-dependent Operations ------*/
	
	/* Data-dependent operations need to read the data table(configured in game mode), 
	* So a world context object is required for getting world
	*/


	/** Add or stack items to a position.
	* If the position is empty, add entry. If the position contains identical items, stack on it. Or fail.
	* This operation is data-dependent, so a world context is needed.
	* @ReturnValue Amount that cannot be added. If failed, return the total amount of input entry.
	*/
	int AddOrStack(UObject* WorldContext, int Position, const FNaItemEntry & Entry);

	/** Add a batch of a certain type of items to the container.
	* @Param Type Type of items to add.
	* @Param Amount Total amount of items.
	* @Param bSearchStackable If true, it will firstly search if there are any same item entries to be stacked on. Or it will add to empty positions only.
	* @Param StartPosition Start position for finding empty positions.
	* @Param bIsUniquified If the items to add are uniquified.
	* @Param UniqName Name if the items are uniquified.
	* @ReturnValue Amount of item that cannot be added to the container.
	*/
	
};



