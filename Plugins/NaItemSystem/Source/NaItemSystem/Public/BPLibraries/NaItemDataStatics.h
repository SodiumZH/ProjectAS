 #pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaItemDataStatics.generated.h"


/**
* A blueprint function library for searching database for item data.
* For non-database-related functions, see NaItemStatics.h.
*/

struct FNaItemDescriptor;
struct FNaItemType;
struct FTableRowBase;


UCLASS()
class NAITEMSYSTEM_API UNaItemDataStatics :public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:

	/* Item type */

	/* Break item type structure and get data */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|ItemType")
	void BreakItemType(const FNaItemType & InType, int & ID, FString & StrName, int & MaxStackingAmount, bool & IsUnique, UDataTable*& UniqueDataTable, TSubclassOf<class AItemEffect>& EffectClass);

	/* Get item type from database from ID. */
	UFUNCTION(BlueprintPure, Category = "NaItemSystem|ItemType")
	FNaItemType GetItemTypeFromID(int ID);

};