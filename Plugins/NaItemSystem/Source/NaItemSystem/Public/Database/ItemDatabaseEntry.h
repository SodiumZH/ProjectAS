#pragma once

#include "CoreMinimal.h"
#include "ItemDatabaseEntry.generated.h"


/* Struct for the item type database. */
USTRUCT()
struct FNaItemTypeDatabaseEntry {

	GENERATED_BODY()

public:

	// Unique ID for the item. DO NOT DUPLICATE!!
	int ID = TEXT("00000");

	// Default name for this item.
	FString Name = TEXT("Item");
	
	int MaxStackingAmount = 64;

	// Whether this item is described by a unique item struct.
	bool bIsUnique = false;

	// If this item is described by a unique item struct,
	// name for identification of this unique item struct.
	class UDataTable* UniqueDataTable = nullptr;

};