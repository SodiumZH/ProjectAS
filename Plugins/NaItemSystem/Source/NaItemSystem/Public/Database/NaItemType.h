#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NaItemType.generated.h"

/* Struct for the item type database.
* The item type data table uses a 7-digit number as row name. 
* Use function IntToRowName() to transform integer to row name.
* Use function RowNameToInt() to transform the 7-digit number to integer.
*/
USTRUCT(BlueprintType)
struct FNaItemTypeDatabaseEntry : public FTableRowBase {

	GENERATED_BODY()

public:

	// Default name for this item.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name = TEXT("Item");
	
	// Max stacking amount
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxStackingAmount = 64;

	// Whether this item is described by a unique item struct.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUnique = false;

	// If this item is described by a unique item struct, the data table for its unique data.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* UniqueDataTable = nullptr;

	// Effect class when using the item
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AItemEffect> EffectClass;

public:

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
struct FNaItemType {

	GENERATED_BODY();

protected:

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ID;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSharedPtr<FNaItemTypeDatabaseEntry> TypeData;

public:

	// Data table reference for searching type data
	static UDataTable * const ItemTypeDataTable;

	// Static item type representing invalid data.
	static FNaItemType DefaultType;

	FNaItemType(int ItemID);

	FNaItemType() :FNaItemType(0) {};

	FORCEINLINE int GetID() const { return ID; };

	static bool IsValidID(int ID) const { return ID >= 0 && ID <= 9999999; };

	FORCEINLINE const FNaItemTypeDatabaseEntry & GetTypeData() const { return *TypeData; };

	FORCEINLINE bool IsValidType() const { return TypeData.IsValid(); };
};
