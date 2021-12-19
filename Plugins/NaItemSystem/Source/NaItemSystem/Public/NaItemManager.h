#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NaItemManager.generated.h"

// This value is intendend to avoid integer overflow.
#define ITEM_CONTAINER_MAX_SIZE 10000


// Struct to discribe an item type.
USTRUCT(BlueprintType)
struct FNaItemDescriptor {

	GENERATED_BODY()

public:

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

};


/* Struct that represents an entry of an item container */
USTRUCT(BlueprintType)
struct FNaItemEntry{

	GENERATED_USTRUCT_BODY()

public:

	/* Constructors */

	/* Default constructor */
	FNaItemEntry() {};




	/*** Properties***/

	/* If true, it means this item socket is empty, ignoring any other properties in this struct. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEmpty = false;




	/* Amount of the item in socket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount = 1;




public:

	// Check if this entry is valid (no error.)
	// NOT IMPLEMENTED. Now always true.
	bool IsValid() { return true; }



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
	TWeakPtr<FNaItemEntry> EntryPtr;
	ENaItemContainerFindingResult Result;

	// Initializer when not found
	enum class NotFoundType :uint8 {
		Empty, OOS, InvID, InvVal, Error
	};

	// Found item successfully
	FNaItemContainerFindingReturn(TWeakPtr<FNaItemEntry> InEntryPtr);

	// Not found
	FNaItemContainerFindingReturn(NotFoundType)

};

typedef TMap<unsigned int, FNaItemEntry> FNaItemContainerContent;
struct FNaItemTypeDatabaseEntry;

/* Structure that contains a series of items */
UCLASS()
struct FNaItemContainer{

	GENERATED_BODY()

protected:

	/* Content as a hash table */
	FNaItemContainerContent Content;

	/* Max size */
	int Size = 64;

public:





	/** Get functions **/

	const int GetSize() { return Size; } const;

	// Give a direct reference to the container content 
	const FNaItemContainerContent & Get() { return Content; } const;

	// Get entry from index
	FNaItemContainerFindingReturn Find(int Index);

	// Get all positions that contains item in given type
	// Return if there is any.
	bool FindItem(int TypeIndex, TArray<int>& Positions);



	/** Operations **/

	/** Add entry. It will copy the entry to container. 
	* If the container position is filled (exists and not labelled empty), the values will not be added unless bForce == true.
	*/
	void AddEntry(int Position, const FNaItemEntry & Entry, bForce = false);

	/** Add a batch of a certain type of item to the container.
	* It will firstly find positions that can stack onto, then empty positions.
	* @Param Type Type of items to add.
	* @Param Amount Total amount of items.
	* @Param PivotPosition Start position for finding empty positions.
	* @Param bIsUniquified If the items to add are uniquified.
	* @Param UniqName Name if the items are uniquified.
	* @ReturnValue Amount of item that cannot be added to the container.
	*/
	int AddItemFromType(const FNaItemTypeDatabaseEntry & Type, int Amount, int PivotPosition = 0, bool bIsUniquified = false, FString UniqName = TEXT(""));



};

UCLASS(Blueprintable)
class UNaItemContainerComponent : public UActorComponent{

	GENERATED_BODY()

	/* Container */



};