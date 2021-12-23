#pragma once

#include "CoreMinimal.h"
#include "NaItemManager.generated.h"

// This value is intended to avoid integer overflow.
#define ITEM_CONTAINER_MAX_SIZE 10000


// Struct to describe an item type.
USTRUCT(BlueprintType)
struct FNaItemDescriptor {

	GENERATED_BODY()

public:

	/********* Properties ********/

	/* Index in item type database. Negative => invalid ID. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemTypeID = 0;

	/* ID in the unique item database. Only unique items (e.g. equipment with random/custom properties) have this ID.
	* For non-unique items, this value is -1, representing it is not recorded in the unique item database.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 UniqueItemID = -1;

	/* Specify which unique item database it is recorded in. For non-unique item it can be nullptr. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDataTable* UniqueItemDatabase = nullptr;

	/* Whether this item is uniquified. If a non-unique item is uniquified, it will be unable to stack with non-uniquified same items.
	* Uniquification of items is identified by name.
	* An example: renamed items in Minecraft.
	*/
	bool bIsUniquified = false;

	// If the item is uniquified, its name.
	FString UniqueName = TEXT("");

public:

	/* Functions */

	// True if the two descriptors are exactly equal (same type, same uniquification)
	bool IsEqual(const FNaItemDescriptor & Other) const;
	bool operator==(const FNaItemDescriptor & Other) const;
	bool operator!=(const FNaItemDescriptor & Other) const;

};


/* Struct that represents an entry of an item container */
USTRUCT(BlueprintType)
struct FNaItemEntry{

	GENERATED_USTRUCT_BODY()

public:

	/* Constructors */

	/* Default constructor */
	FNaItemEntry() {};

	// Const version of copy constructor
	FNaItemEntry(const FNaItemEntry & CopyFrom);


	/*** Properties***/

	/* Descriptor of the type */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNaItemDescriptor TypeDescriptor;

	/* Amount of the item in socket .
	* It should never be zero. When it turns zero the entry should be deleted.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount = 1;




public:



	// Check if this entry is valid (no error.)
	bool IsValid();



};











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


/* Return values for finding in a container */

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

};

typedef TMap<int, TSharedPtr<FNaItemEntry>> FNaItemContainerContent;
struct FNaItemTypeDatabaseEntry;

/* Structure that contains a series of items */
USTRUCT(BlueprintType)
struct FNaItemContainer{

	GENERATED_BODY()

protected:

	/* Content as a hash table */
	FNaItemContainerContent Content;

	/* Max size */
	int Size = 64;

public:


	/** Get functions **/

	// Get the size.
	FORCEINLINE int GetSize() const { return Size; };

	// Check if a position is in size. Please note that the valid values is [0,Size) just like an array.
	FORCEINLINE bool IsInSize(int Position) const { return Position >= 0 && Position < Size; };

	// Get entry from index
	FNaItemContainerFindingReturn Find(int Index);

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

	/* Swap two entries. This action will not fail if no OOS. */
	void SwapEntry(int P1, int P2);

	/** Add a batch of a certain type of item to the container.
	* It will firstly find positions that can stack onto, then empty positions.
	* @Param Type Type of items to add.
	* @Param Amount Total amount of items.
	* @Param PivotPosition Start position for finding empty positions.
	* @Param bIsUniquified If the items to add are uniquified.
	* @Param UniqName Name if the items are uniquified.
	* @ReturnValue Amount of item that cannot be added to the container.
	*/
	//int AddItemFromType(const FNaItemTypeDatabaseEntry & Type, int Amount, int PivotPosition = 0, bool bIsUniquified = false, FString UniqName = TEXT(""));



};

UCLASS(Blueprintable)
class UNaItemContainerComponent : public UActorComponent{

	GENERATED_BODY()

	/* Container */



};