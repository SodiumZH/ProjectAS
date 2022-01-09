#pragma once

#include "CoreMinimal.h"
#include "NaItemEntry.generated.h"

/**
* NaItemEntry is a struct for describing a socket content
* Including the full item data and the amount.
* Some item properties needs to be referred to the item type database. (see /Database/ItemTypeDatabaseEntry.h)
**/



// Struct to describe an item type.
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemDescriptor {

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

	/* Whether this item is uniquified. If a non-unique item is uniquified, it will be unable to stack with non-uniquified same items.
	* Uniquification of items is identified by name.
	* An example: renamed items in Minecraft.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsUniquified = false;

	// If the item is uniquified, its name.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UniqueName = TEXT("");

public:

	/* Functions */

	// True if the two descriptors are absolutely identical (same type, same unique ID, same uniquification)
	bool IsEqual(const FNaItemDescriptor & Other) const;
	bool operator==(const FNaItemDescriptor & Other) const;
	bool operator!=(const FNaItemDescriptor & Other) const;

	/* Constructors */
	FNaItemDescriptor(int InTypeID = 0, int InUniqueItemID = -1, bool InIsUniquified = false, FString InUniqueName = TEXT("")) :
		ItemTypeID(TypeID),
		UniqueItemID(InUniqueItemID),
		bIsUniquified(InIsUniquified),
		UniqueName(InUniqueName)
	{};


};


/* Struct that represents an entry of an item container */
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemEntry {

	GENERATED_USTRUCT_BODY()

public:




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

	/* Constructors */

	// Default constructor
	FNaItemEntry() : TypeDescriptor(FNaItemDescriptor()) {};

	// Const version of copy constructor
	FNaItemEntry(const FNaItemEntry & CopyFrom);
	
	// Check if this entry is valid (no error.)
	bool IsValid();

};



/**
* NaActiveItemEntry is an item entry which can interact with a container.
* e.g. fill, exchange, take, ...
* Mainly for description of the item entry selected by the mouse.
*/
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaActiveItemEntry : public FNaItemEntry {

	GENERATED_BODY()

public:

	/* Constructors */

	FNaActiveItemEntry() :
		FNaItemEntry() 
	{};

	//FNaActiveItemEntry(const FNaActiveItemEntry & CopyFrom);

public:

	// If true, this item entry is occupied. If false, it will be regarded as empty despite other factors.
	bool bIsOccupied = false;



};
