#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaItemEffect.generated.h"

class AActor;

// Types of consumption on item usage.
// Consumption needs additionally an int (IntParam) and a NaItemEntry array (EntryArray).
UENUM(BlueprintType)
enum class ENaItemUsageConsumptionType:uint8 {

	// Consume one item
	IUCT_One UMETA(DisplayName = "Consuming One"),

	// Consume more than 1 items
	// IntParam - Consuming amount
	IUCT_Multi	UMETA(DisplayName = "Consuming Multiple"),

	// No consumption
	IUCT_None UMETA(DisplayName = "No Consumption"),

	// Consume the item and replace the emptied slot with another item. 
	// Applied on e.g. equipments, replacing the equipment with player's wearing one
	// Only applied on items not stackable (i.e. MaxStackingAmount == 1).
	// EntryArray[0] - Item entry to replace
	IUCT_Replace UMETA(DisplayName = "Replacing"),

	// Consume not this but other items, e.g. recipes
	// EntryArray - Other items to consume
	IUCT_Other UMETA(DisplayName = "Consuming Other Items"),

	// Consume other items and this item
	// EntryArray - Other items to consume
	IUCT_OtherAndThis	UMETA(DisplayName = "Consuming Other Items and This"),

	// Consume other items and multiple this items
	// IntParam - Consuming amount
	// EntryArray - Other items to consume
	IUCT_OtherAndMultiThis	UMETA(DisplayName = "Consuming Other Items and Multiple This"), 

	// Consume one this and get other items
	// EntryArray - Other items to get
	IUCT_GetOther	UMETA(DisplayName = "Consuming This and Get Other Items"),

	// Get other item without consuming this
	// EntryArray - Other items to get
	IUCT_GetOtherNoConsumption	UMETA(DisplayName = "Get Other Items without Consumption"),

	// Consume multiple this and get other items
	// IntParam - Consuming amount
	// EntryArray - Other items to get
	IUCT_GetOtherConsumingMulti	UMETA(DisplayName = "Consuming Multiple This and Get Other Items"),

	// Custom type. Should be defined in corresponding item effect
	IUCT_Custom	UMETA(DisplayName = "Custom")
};
/*
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API ENaItemContainerUsageResult {

	GENERATED_BODY()

public:
	
	// Whether usage succeeded
	UPROPERTY(BlueprintReadWrite)
	bool bSucceeded = false;
	ENaItemContainerUsageResult SetSucceeded(bool value);

	// Amount of items consumed per usage
	UPROPERTY(BlueprintReadWrite)
	int ConsumedAmount = 0;
	ENaItemContainerUsageResult SetConsumedAmount(int value);

	// Usage failed, nothing happened
	static ENaItemContainerUsageResult Null();
	
	// Usage succeeded and an item was consumed
	static ENaItemContainerUsageResult Consumed();

	// Usage succeeded but not consumed
	static ENaItemContainerUsageResult UsedNoConsumption();

};
*/

// When item is used from a container, the usage result
UENUM()
enum class ENaItemContainerUsageResult :uint8 {

	// Usage succeeded
	ICUR_Succeeded	UMETA(DisplayName = "Succeeded"),

	// The item to use is not enough
	ICUR_NotEnoughItem	UMETA(DisplayName = "NotEnoughItem"),

	// The item to use is enough, but additional other items to consume are not enough
	ICUR_NotEnoughAdditionItems	UMETA(DisplayName = "NotEnoughOtherItems"),

	// The item will give another items to the container, but the space of container is not enough
	ICUR_NotEnoughSpace	UMETA(DisplayName = "ContainerSpaceNotEnough"),

	// Unidentified failure
	ICUR_Failed	UMETA(Display = "Unidentified Failure")
};


/* Effect class when using items */
// Server only
UCLASS(Blueprintable, BlueprintType)
class NAITEMSYSTEM_API UNaItemEffect :public UObject {

	GENERATED_BODY()

public:

	// Item effect defined in cpp.
	// @Param ItemID Item ID to be used.
	// @Param SourceActor Actor which uses the item.
	// @Param TargetActor Actor as target of the item.
	// @Param ItemPosition If the item comes from an item container, its position in the container. -1 means disabled.
	// @ReturnValue Whether the usage succeeded.
	//virtual ENaItemContainerUsageResult ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);

	// If true, the effect will use blueprint function override (i.e. use ItemEffectBP() instead of ItemEffect()).
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemEffect")
//	bool bUseBlueprintOverride = false;

	/* Item effect defined in BP.
	* WARNINGS
	* Keep in mind to ensure the container item stacking amounts are correct! If not, it may trigger asserts or cause unexpected behaviors.
	* Consumption or adding items should be especially noticed. When adding item, always check if the container is full!
	* @Param ItemID Item ID to be used.
	* @Param SourceActor Actor which uses the item.
	* @Param TargetActor Actor as target of the item.
	* @Param ItemPosition If the item comes from an item container, its position in the container. -1 means disabled.
	* @ReturnValue Usage result, including whether successful, how many consumed.
	* For tutorial and warnings, see NaItemEffect.h
	**/
	UFUNCTION(BlueprintNativeEvent, DisplayName = "ItemEffectBlueprintOverride", Category = "NaItemSystem|ItemEffect")
	ENaItemContainerUsageResult ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);
	virtual ENaItemContainerUsageResult ItemEffect_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);

public:

	ENaItemContainerUsageResult UseItem(UObject* WorldContext, int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition);

};

/* ItemEffect setting tutorial
* Item ID is the item expected to use. This means multiple items can be defined in a single effect class.
* SourceActor is the item used from (usually the player, must be valid). Invalid source actor triggers assert. Keep in mind that what exactly the source actor is, player controller or pawn.
* TargetActor is the item used toward. E.g. Other players, monsters, etc. Optional, allowed to be invalid. (Caution for nullptr error!)
* Position: Only for when the item is used from a container. This input is applied to define position-related behaviors. Also, this value can serve as an additional parameter, not limited in the container position.
* return
* 
* WARNINGS
* Keep in mind to ensure the container item stacking amounts are correct! If not, it may trigger asserts or cause unexpected behaviors. 
* Consumption or adding items should be especially noticed. When adding item, always check if the container is full!
*/