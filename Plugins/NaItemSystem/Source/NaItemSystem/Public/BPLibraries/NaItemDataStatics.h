 #pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Data/NaItemEffectdata.h"
#include "../NaItemType.h"
#include "NaItemDataStatics.generated.h"


/**
* A blueprint function library for searching database for item data.
* For non-database-related functions, see NaItemStatics.h.
*/

struct FTableRowBase;
class UDataTable;
class ANaItemEffect;

UCLASS()
class NAITEMSYSTEM_API UNaItemDataStatics :public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	/*- Item type data table -*/

	/* Get item type from database by name. */
	UFUNCTION(BlueprintPure, meta = (Keywords = "get na item type make na item type"), Category = "NaItemSystem|ItemType")
	static TOptional<UNaItemType*> GetItemTypeFromName(UObject* WorldContext, FName name);

};