#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NaItemEffect.h"
#include "NaItemType.generated.h"

/* Struct for the item type database. This is the item for distinction of items.
* This struct is directly applied by the item container for its behaviors.
* The item data table uses a 7-digit number as row name. E.g. "0012345"
* Use function IntToRowName() to transform integer to row name. E.g. 123 => "0000123"
* Use function RowNameToInt() to transform the 7-digit number to integer. E.g. "0054321" => 54321 
*/
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemTypeData : public FTableRowBase {

	GENERATED_BODY()

public:

	// Default name for this item.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name = TEXT("Item");
	
	// Max stacking amount
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxStackingAmount = 64;

	// Whether this item is described by a unique item struct.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsUnique = false;

	// If this item is described by a unique item struct, the data table for its unique data.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* UniqueDataTable = nullptr;

public:

	// From variables
	FNaItemTypeData(FString InName = TEXT("Item"), int InStackingAmount = 64, bool bInIsUnique = false, UDataTable* InUniqueDataTable = nullptr);

	// Copy from other
	FNaItemTypeData(const FNaItemTypeData & CopyFrom);

	/* Functions for transformation between integers and 7-digit numerical row names */

	// Check if the name is a valid row name.
	// Since it cost extra resource, it can be called in WITH_EDITOR.
	// Warning: 0000000 represents default item and is not valid.
	static bool IsValidRowName(FName InRowName);

	// Transform integer to row name
	static FName IntToRowName(int ID);

	// Transform row name to integer
	static int RowNameToInt(FName InRowName);

};


/* A descriptor of item type.
* Always use this struct to interact with other classes/structs.
*/
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemType {

	GENERATED_BODY();

protected:

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ID;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSharedPtr<FNaItemTypeData> TypeData;

public:

	// Make from ID and data ptr
	FNaItemType(int InID, TSharedPtr<FNaItemTypeData> InData);

	// Make an invalid default type
	FNaItemType() :FNaItemType(0, nullptr) {};

	// Get ID as int
	FORCEINLINE int GetID() const { return ID; };

	// Check if an int ID is available for an ID
	static bool IsValidID(int ID) { return ID >= 0 && ID <= 9999999; };

	// Check if type data is valid
	FORCEINLINE bool IsValid() const { return TypeData.IsValid(); };

	// Get a ref of type data
	FORCEINLINE const FNaItemTypeData & GetTypeData() const { return *TypeData; };

	// Get a copy of type data
	void CopyTypeData(FNaItemTypeData & OutData) const;

	TSharedPtr<FNaItemTypeData> CopyTypeData() const;

	// Check if a type is valid
	FORCEINLINE bool IsValidType() const { return TypeData.IsValid(); };

};
