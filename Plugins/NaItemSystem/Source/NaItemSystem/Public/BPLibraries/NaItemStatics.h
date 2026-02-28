#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaItemStack.h"
#include "NaItemStatics.generated.h"


/**
* This is the blueprint function interface for
* general item functions.
* For database-related functions, see NaItemDataStatics.h.
*/

class UNaGameModeItemSystemComponent;

UCLASS()
class NAITEMSYSTEM_API UNaItemStatics :public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	/*- Item System game mode component related -*/
	UFUNCTION(BlueprintCallable, Category = "NaItemSystem|GameMode")
	static UNaGameModeItemSystemComponent * GetGameModeItemSystemComponent(UObject* WorldContext);

};