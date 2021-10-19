#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../NaGlobalHeader.h"
#include "NaItemManager.generated.h"


/* Struct that represents a socket */
USTRUCT(Blueprintable)
struct FNaItemSocket {

	GENERATED_USTRUCT_BODY()

public:

	/* If true, it means this item socket is empty, ignoring any other properties in this struct. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEmpty = false;

	/* Index in item type database */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemTypeID = 0;

	/* Display name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName = TEXT("Item");

	/* Amount of the item in socket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount = 1;

	/* ID in the unique item database. Only unique items (e.g. equipment with random/custom properties) have this ID.
	* For non-unique items, this value is -1, representing it is not recorded in the unique item database.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 UniqueItemID = -1;

	/* Specify which unique item database it is recorded in. For non-unique item it can be ignored. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UniqueItemDatabaseName = NAME_None;


	static const FNaItemSocket EmptySocket = (true, )
};

UCLASS(Blueprintable)
class NAPACK_API UNaItemContainerComponent : public UActorComponent{

	GENERATED_BODY()

public:

	TMap<int, FNaItemSocket> Container;



}