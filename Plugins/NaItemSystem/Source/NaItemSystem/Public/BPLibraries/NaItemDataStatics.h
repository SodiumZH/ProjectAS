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
class /*NAITEMSYSTEM_API*/ UNaItemStatics :public UBlueprintFunctionLibrary {
	GENERATED_BODY()
public:

};