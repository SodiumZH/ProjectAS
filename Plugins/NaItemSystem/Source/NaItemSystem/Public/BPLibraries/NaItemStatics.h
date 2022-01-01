#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NaItemContainer.h"
#include "NaItemStatics.generated.h"


/**
* This is the blueprint function interface for
* general item functions.
* For database-related functions, see NaItemDataStatics.h.
*/

struct FNaItemDescriptor;
struct FNaItemType;
struct FTableRowBase;
struct FNaItemEntry;

UCLASS()
class /*NAITEMSYSTEM_API*/ UNaItemStatics :public UBlueprintFunctionLibrary {

	GENERATED_BODY()

public:

	/*** NaItemDescriptor ***/

	static bool IsEqual_ItemDescriptor(const FNaItemDescriptor & A, const FNaItemDescriptorr & B);
	
	/*** NaItemEntry ***/

	static bool IsValidItemEntry(const FNaItemEntry & ItemEntry);


	/*** NaItemContainer ***/

	static int GetItemContainerSizeLimit() { return ITEM_CONTAINER_MAX_SIZE; };

	static int GetSize(const FNaItemContainer & Target) { return Target.GetSize(); };

	static bool IsInSize(const FNaItemContainer & Target) { return Target.IsInSize(); };

	static void Resize(TSharedPtr<FNaItemContainer> Target, int NewSize, bool bForce) { return Target->Resize(NewSize, bForce); };

	static void FindFromIndex(const FNaItemContainer & Target, int Position, FNaItemContainerFindingReturn & ReturnType, TSharedPtr<FNaItemEntry> & Result);


};