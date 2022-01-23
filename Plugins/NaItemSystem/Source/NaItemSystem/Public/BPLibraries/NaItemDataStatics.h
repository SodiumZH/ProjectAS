 #pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Database/NaItemType.h"
#include "../NaItemContainer.h"
#include "../NaItemEntry.h"
#include "NaItemDataStatics.generated.h"


/**
* A blueprint function library for searching database for item data.
* For non-database-related functions, see NaItemStatics.h.
*/

struct FNaItemDescriptor;
struct FTableRowBase;
class UDataTable;
class ANaItemEffect;

UCLASS(Config=Game)
class NAITEMSYSTEM_API UNaItemDataStatics :public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	/*- Item type -*/


	/* Break item type structure and get data */
	UFUNCTION(BlueprintPure, meta = (AdvancedDisplay = 7, Keywords = "break na item type"), Category = "NaItemSystem|ItemType")
	static void BreakItemType(const FNaItemType & InType, int & ID, FString & StrName, int & MaxStackingAmount, bool & bIsUnique, UDataTable*& UniqueDataTable, TSubclassOf<class ANaItemEffect>& EffectClass, FName & RowName);

	/* Get item type from database from ID. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "get na item type make na item type"), Category = "NaItemSystem|ItemType")
	static FNaItemType GetItemTypeFromID(int ID);

	/*- Item descriptor -*/

	/* Make default item descriptor from ID */
	UFUNCTION(BlueprintPure, meta = (Keywords = "make na item descriptor"), Category = "NaItemSystem|ItemType")
	static FNaItemDescriptor MakeDefaultDescriptor(int ID);

};