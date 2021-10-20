#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../NaGlobalHeader.h"
#include "NaItemManager.generated.h"

#define ITEM_CONTAINER_MAX_SIZE 10000

/* Struct that represents a socket */
USTRUCT(Blueprintable)
struct FNaItemSocket{

	GENERATED_USTRUCT_BODY()

public:

	/* If true, it means this item socket is empty, ignoring any other properties in this struct. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEmpty = false;

	/* Index in item type database. Negative => invalid ID. */
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

public:

	static NAPACK_API const FNaItemSocket EmptySocket;

	// Check if an item socket struct is valid. Currently it's always true. Code in NaItemSystem/NaItemManager.h or *.cpp to reset.
	UFUNCTION(BlueprintPure, DisplayName = "Is Valid (Item Socket)", Category = "NaPack|NaItemSystem")
	bool IsValidSocket() { return true; };

};

UENUM(BlueprintType)
enum class ENaItemContainerBrowseResult :uint8 {
	ICBR_Filled		UMETA(DisplayName = "Filled"),			// Browsed successfully, and the socket is containing valid item(s).
	ICBR_Empty		UMETA(DisplayName = "Empty")			// Browsed successfully, and the socket is empty.
	ICBR_OOS		UMETA(DisplayName = "Out of Size"),		// Trying to browse at the position out of the max size
	ICBR_InvID		UMETA(DisplayName = "Invalid Index"),	// Trying to browse at invalid position e.g. zero or negative
	ICBR_InvVal 	UMETA(DisplayName = "Invalid Item Record")		// Found record correctly, but the item record itself is invalid (containing something wrong)
	//ICBR_Error		UMETA(DisplayName = "Error")			// Unexpected error
};

UCLASS(Blueprintable)
class NAPACK_API UNaItemContainerComponent : public UActorComponent{

	GENERATED_BODY()

	/* Container */

protected:

	/* ID starts from 1. */
	TMap<int, FNaItemSocket> Container;

	// Max container size
	int MaxSize = 8;

	// Create & fill container. Only called on construction.
	void InitializeContainer();

public:

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Get Max Size (Item Manager)"), Category = "NaPack|ItemSystem")
	int GetMaxSize() { return MaxSize; };

	// Return whether the input index is valid for the container.
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Is Valid Index (Item Manager)"), Category = "NaPack|ItemSystem")
	bool IsValidIndex(int Index);

	/* Add an item to container. This action doesn't change the size of container.
	* @Param AllowReplacement If true, the new input item socket will replace the existing socket at the index. Else the item will be added only when the index is empty.
	* @ReturnValue Whether the item is successfully added.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Add Item (Item Manager)"), Category = "NaPack|ItemSystem")
	bool AddItem(int Index, const FNaItemSocket& InItem, bool AllowReplacement = true);

	/* Resize the container. 
	* If it's expanding, the additional sockets will be empty. If shrinking, it will works only when the shrinked sockets are all empty or ForceShrink == true.
	* @Param ForceShrink If true, shrinking will work when shrinked sockets are not all empty. In this case the shrinked item information will be lost.
	* @ReturnValue Whether resizing is successfully executed. True if NewSize == OldSize.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Resize (Item Manager)"), Category = "NaPack|ItemSystem")
	bool Resize(int NewSize, bool ForceShrink = false);

	/* Check item at a certain index. Return an enum for its correctness and error type.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Check Item (Item Container)", Keywords = "valid correct right"), Category = "NaPack|ItemSystem")
	ENaItemContainerBrowseResult CheckItem(int Index);

	/* Check if the container's elements are correct. Return true if all correct, and false if something is wrong.
	* @Param ErrorIndex Index at which the element is wrong. Empty if all correct.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Check Container (Item Container)", Keywords = "valid correct right"), Category = "NaPack|ItemSystem")
	bool CheckContainer(TArray<int>& ErrorIndex);

	/* Get item at a certain index and copy to OutItem. Return an enum for its correctness and error type.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Get Item (Item Container)", Keywords = "browse get item"), Category = "NaPack|ItemSystem")
	ENaItemContainerBrowseResult GetItem(int Index, FNaItemSocket& OutItem);

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, DisplayName = "Swap Items (Item Container)", Keywords = "exchange"), Category = "NaPack|ItemSystem")
	bool Swap(int Index1, int Index2);

}