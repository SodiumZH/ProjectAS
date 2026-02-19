 #pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Data/NaItemType.h"
#include "../Data/NaItemDisplaydata.h"
#include "../Data/NaItemEffectdata.h"
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

UCLASS()
class NAITEMSYSTEM_API UNaItemDataStatics :public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	/* Row name related */

	/* Make item row name (7-digit numerical string) from integer ID. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "row name id index"), Category = "NaItemSystem|Data")
	static FName ToItemRowName(int InIndex);

	/* Get integer ID from item row name (7-digit numerical string). */
	UFUNCTION(BlueprintPure, meta = (Keywords = "row name id index"), Category = "NaItemSystem|Data")
	static int ToIndex(FName InItemRowName);

	/*- Item type data table -*/

	/* Break item type structure and get data */
	UFUNCTION(BlueprintPure, meta = (AdvancedDisplay = 6, Keywords = "break na item type"), Category = "NaItemSystem|ItemType")
	static void BreakItemType(const FNaItemType & InType, int & ID, FString & StrName, int & MaxStackingAmount, bool & bIsUnique, UDataTable*& UniqueDataTable, FName & RowName);

	/* Get item type from database from ID. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "get na item type make na item type"), Category = "NaItemSystem|ItemType")
	static FNaItemType GetItemTypeFromID(UObject* WorldContext, int ID);

	/* Get item display data from ID */
	UFUNCTION(BlueprintPure, meta = (Keywords = "get display data"), Category = "NaItemSystem|ItemType")
	static FNaItemDisplayData GetItemDisplayDataFromID(UObject* WorldContext, int ID);

	/* Get item effect data from ID */
	UFUNCTION(BlueprintPure, meta = (Keywords = "get effect usage use using data"), Category = "NaItemSystem|ItemType")
	static FNaItemEffectData GetItemEffectDataFromID(UObject* WorldContext, int ID);

	/*- Item descriptor -*/

	/* Make default item descriptor from ID */
	UFUNCTION(BlueprintPure, meta = (Keywords = "make na item descriptor"), Category = "NaItemSystem|ItemType")
	static FNaItemDescriptor MakeDefaultDescriptor(int ID);


	

};