#pragma once

#include "CoreMinimal.h"
#include "NaItemStack.h"
#include "NaItemEffect.h"
#include "NaItemContainer.generated.h"

// This value is intended to avoid integer overflow.
#define ITEM_CONTAINER_MAX_SIZE 10000

class UNaGameModeItemSystemComponent;


/* Result types for browsing an item container */
UENUM()
enum class ENaItemContainerFindingResult :uint8 {
	ICFR_Filled		UMETA(DisplayName = "Filled"),			// Browsed successfully, and the socket is containing valid item(s).
	ICFR_Empty		UMETA(DisplayName = "Empty"),		// Browsed successfully, and the socket is empty.
	ICFR_OOS		UMETA(DisplayName = "Out of Size"),		// Trying to browse at the position out of the max size
	/* The above values are correct results */
	/* The below values are errors that should not appear */
	ICFR_InvID		UMETA(DisplayName = "Invalid Index"),	// Trying to browse at invalid position (negative or out of ITEM_CONTAINER_MAX_SIZE defined in NaItemManager.h)
	ICFR_InvVal 	UMETA(DisplayName = "Invalid Item Record"),		// Found record correctly, but the item record itself is invalid (containing something wrong)
	ICFR_Error		UMETA(DisplayName = "Error")			// Unexpected error
};


/* Return values for Searching in a container */
struct FNaItemContainerFindingReturn {
	// Values
	UNaItemStack* Stack;
	ENaItemContainerFindingResult Result;

	// Initializer when not found
	enum class NotFoundType :uint8 {
		Empty, OOS, InvID, InvVal, Error
	};

	// Found item successfully
	FNaItemContainerFindingReturn(UNaItemStack* InStack);

	// Not found
	FNaItemContainerFindingReturn(NotFoundType);

	// Copy
	FNaItemContainerFindingReturn(const FNaItemContainerFindingReturn& Copy) :
		Stack(Copy.Stack),
		Result(Copy.Result)
	{};
};

UENUM()
enum class ENaItemContainerAddingCheckResult {
	ICACR_CanAdd,
	ICACR_CanPartlyAdd,
	ICACR_CannotAdd

	
};


/* Structure that contains a series of items.
* It can be used to describe a bag, shop, etc.
*/
USTRUCT(Blueprintable)
struct NAITEMSYSTEM_API FNaItemContainer {

	GENERATED_BODY()

protected:

	/* Content as an array of item stacks. Null entries represent empty slots. */
	UPROPERTY()
	TArray<TObjectPtr<UNaItemStack>> Content;

	/* Max size */
	int Size = 64;



// Check if Content.Num() == Size. CRASH when failed.
#define CheckSize() checkf(Content.Num() == Size, TEXT("NaItemContainer Error: Container size != Content array actual size."))


public:

	/***** Constructors *****/

	// Make an empty container. Default size is 64.
	FNaItemContainer();

	// Make empty container with given size.
	FNaItemContainer(int InSize);

	// Copy from other. 
	FNaItemContainer(const FNaItemContainer& CopyFrom);

	// Make from initial content map and size
	FNaItemContainer(int InSize, const TMap<int, UNaItemStack*>& InitialContent);

	/** Resize the container.
	* @Param bForce If true, when shrinked area contains items, it will ignore them (causing the items lost!). Or it will fail if shrinked area contains items.
	*/
	bool Resize(int NewSize, bool bForce = false);

	/*----------------------------------------------------*/
	/** Get functions **/

	// Get the size.
	FORCEINLINE int GetSize() const { return Size; };

	// Check if a position is in size. Please note that the valid values is [0,Size) just like an array.
	FORCEINLINE bool IsInSize(int Position) const { return Position >= 0 && Position < Size; };

	// Get entry from index
	FNaItemContainerFindingReturn Find(int Index) const;

	/** Get all positions that contain items of a given type.
	* Return whether there are any.
	* @Param Type The item type to search for.
	* @Param Positions Return positions containing the item of given type.
	*/
	bool FindByType(UNaItemType* Type, TArray<int>& Positions);


	/** Operations **/

	/** Add item stack at position. 
	* If the container position is occupied, the values will not be added unless bForce == true.
	* Return whether succeeded.
	*/
	bool AddEntry(int Position, UNaItemStack* Stack, bool bForce = false);


	/* Remove an item from position. */
	void RemoveEntry(int Position);

	/* Move an item entry to another position.
	* @Param bForce If true, the "from" entry will overwrite the "to" entry if the latter is occupied.
	* @ReturnValue Whether moved successfully. Fail if moving from an empty position or to an occupied position without bForce == true. 
	*/
	bool MoveEntry(int From, int To, bool bForce = false);

	/* Swap two entries. This action will not fail if no Out Of Size. */
	void SwapEntry(int P1, int P2);
	

	/** Maintainance **/

	// Find all positions with invalid/empty stacks and reset them to null
	void ClearInvalid();




	/*----- Data-dependent Operations ------*/

	/** Add or stack items to a position.
	* If the position is empty, add stack. If the position contains the same item type, stack on it. Or fail.
	* @ReturnValue Amount that cannot be added. If failed, return the total amount of input stack.
	*/
	int AddOrStack(int Position, UNaItemStack* Stack);


	/* --- Operations not specifying positions --- */

	/** Give a stack to the container, not specifying the position.
	* @ ReturnValue Amount that cannot be added. If failed, return the total amount of input stack.
	**/
	int GiveItem(UNaItemStack* Stack);

	/** GiveItem function that executes only when all items can be given.
	* @ ReturnValue Whether succeeded.
	**/
	bool GiveItemComplete(UNaItemStack* Stack);

	/** Check if a stack can be completely added to the container.
	* This function doesn't really add items.
	**/
	bool CanGiveItemComplete(const UNaItemStack* Stack) const;

	/** Check if multiple stacks can all be completely added.
	* This function doesn't really add items.
	**/
	bool CanGiveItemMultiComplete(const TArray<UNaItemStack*>& Stacks);



	/*--- Item Usage ---*/
	
	// Check if the container is correct and item can be used before usage; actions before usage
	virtual ENaItemContainerUsageResult PreUsageProcess(UObject* WorldContext, int Position, class AActor* Source, AActor* Target);

	// Action of usage of item itself
	virtual ENaItemContainerUsageResult ExecuteUseItem(UObject* WorldContext, int Position, class AActor* Source, AActor* Target, ENaItemContainerUsageResult PreUsageResult);

	// Actions after usage
	virtual ENaItemContainerUsageResult PostUsageProcess(UObject* WorldContext, int Position, class AActor* Source, AActor* Target, ENaItemContainerUsageResult UsageResult);

	// Function actually called for usage
	virtual ENaItemContainerUsageResult UseItem(UObject* WorldContext, int Position, class AActor* Source, AActor* Target);
		

	virtual ~FNaItemContainer() {};
};



