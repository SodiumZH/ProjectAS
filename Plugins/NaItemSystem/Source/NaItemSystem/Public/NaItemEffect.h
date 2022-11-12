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
	IUCT_Replace_NOT_IMPLEMENTED UMETA(DisplayName = "Replacing NOT IMPLEMENTED"),

	// Consume not this but other items, e.g. recipes
	// EntryArray - Other items to consume
	IUCT_Other_NOT_IMPLEMENTED UMETA(DisplayName = "Consuming Other Items NOT IMPLEMENTED"),

	// Consume other items and this item
	// EntryArray - Other items to consume
	IUCT_OtherAndThis_NOT_IMPLEMENTED	UMETA(DisplayName = "Consuming Other Items and This NOT IMPLEMENTED"),

	// Consume other items and multiple this items
	// IntParam - Consuming amount
	// EntryArray - Other items to consume
	IUCT_OtherAndMultiThis_NOT_IMPLEMENTED	UMETA(DisplayName = "Consuming Other Items and Multiple This NOT IMPLEMENTED"),

	// Consume one this and get other items
	// EntryArray - Other items to get
	IUCT_GetOther_NOT_IMPLEMENTED	UMETA(DisplayName = "Consuming This and Get Other Items NOT IMPLEMENTED"),

	// Get other item without consuming this
	// EntryArray - Other items to get
	IUCT_GetOtherNoConsumption_NOT_IMPLEMENTED	UMETA(DisplayName = "Getting Other Items without Consumption NOT IMPLEMENTED"),

	// Consume multiple this and get other items
	// IntParam - Consuming amount
	// EntryArray - Other items to get
	IUCT_GetOtherConsumingMulti_NOT_IMPLEMENTED	UMETA(DisplayName = "Consuming Multiple This and Get Other Items NOT IMPLEMENTED"),

	// Custom type. Should be defined in corresponding item effect
	IUCT_Custom	UMETA(DisplayName = "Custom")
};


// When item is used from a container, the usage result
UENUM()
enum class ENaItemContainerUsageResult :uint8 {

	/*------------- Errors -------------*/

	// Unidentified failure
	ICUR_Failed	UMETA(Display = "UnidentifiedFailure"),

	// Internal error, usually from C++ code
	ICUR_Error	UMETA(Display = "InternalError"),

	// Using items undefined in data table
	ICUR_Invalid	UMETA(Display = "UsingItemWithInvalidIndex"),

	/*------------- Succeeded -------------*/

	// Usage succeeded
	ICUR_Succeeded	UMETA(DisplayName = "Succeeded"),


	/*------------- Normal failures from item properties -------------*/
	/*------------- index starts from 10 -------------*/

	// Item to use is defined non-usable in Effect Data Table
	ICUR_NotUsable = 10	UMETA(DisplayName = "NotUsable"),

	// Attempting to use an empty item
	ICUR_Empty	UMETA(DisplayName = "UsingEmptyItem"),


	/*------------- Normal failures from container -------------*/
	/*------------- index start from 20 -------------*/
	// The item to use is not enough
	ICUR_NoEnoughItem = 20	UMETA(DisplayName = "NoEnoughItem"),

	// The item to use is enough, but additional other items to consume are not enough
	ICUR_NoEnoughAdditionalItems	UMETA(DisplayName = "NoEnoughOtherItems"),

	// The item will give another items to the container, but the space of container is not enough
	ICUR_NoEnoughSpace	UMETA(DisplayName = "NoEnoughContainerSpace"),


	/*------------- Custom failure types -------------*/
	/*------------- index start from 100 -------------*/

	ICUR_CustomFailure0 = 100	UMETA(DisplayName = "Custom Failure Type 0"),
	ICUR_CustomFailure1	UMETA(DisplayName = "Custom Failure Type 1"),
	ICUR_CustomFailure2	UMETA(DisplayName = "Custom Failure Type 2"),
	ICUR_CustomFailure3	UMETA(DisplayName = "Custom Failure Type 3"),
	ICUR_CustomFailure4	UMETA(DisplayName = "Custom Failure Type 4"),
	ICUR_CustomFailure5	UMETA(DisplayName = "Custom Failure Type 5"),
	ICUR_CustomFailure6	UMETA(DisplayName = "Custom Failure Type 6"),
	ICUR_CustomFailure7	UMETA(DisplayName = "Custom Failure Type 7"),
	ICUR_CustomFailure8	UMETA(DisplayName = "Custom Failure Type 8"),
	ICUR_CustomFailure9	UMETA(DisplayName = "Custom Failure Type 9")

};


/* Effect class when using items */
// Server only
UCLASS(Blueprintable, BlueprintType)
class NAITEMSYSTEM_API UNaItemEffect :public UObject {

	GENERATED_BODY()

public:

	/* Item effect applied.
	* When the item type can be used, return Succeeded without considering the container status. 
	* Usage in the container will double check its usability, and is not defined here.
	* @Param ItemID Item ID to be used.
	* @Param SourceActor Actor which uses the item.
	* @Param TargetActor Actor as target of the item.
	* @Param ItemPosition If the item comes from an item container, its position in the container. -1 means disabled.
	* @ReturnValue Usage result, including whether successful, how many consumed.
	* For tutorial and warnings, see NaItemEffect.h
	**/
	UFUNCTION(BlueprintNativeEvent, DisplayName = "ItemEffect", Category = "NaItemSystem|ItemEffect")
	ENaItemContainerUsageResult ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);
	virtual ENaItemContainerUsageResult ItemEffect_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);

public:

	ENaItemContainerUsageResult UseItem(UObject* WorldContext, int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition);

};

/* ItemEffect setting tutorial
* Item ID is the item expected to use. This means multiple items can be defined in a single effect class.
* SourceActor is the item used from (usually the player, must be valid). Invalid source actor triggers assert. Keep in mind that what exactly the source actor is, player controller or pawn.
* TargetActor is the item used toward. E.g. Other players, monsters, etc. Optional. (Caution for nullptr error!)
* Position: Only for when the item is used from a container. This input is applied to define position-related behaviors. Also, this value can serve as an additional parameter, not limited in the container position.
* return: Whether this item type is usable, and why it's not usable. DO NOT CONSIDER CONTAINER!!
*/

/** Example
	If we want to implement an HP potion recovering 50 HP each use, as ID == 10

	In ItemEffect override (for BP, override ItemEffect()):
class UMyEffect: public UNaItemEffect
{
	//....
	virtual ENaItemContainerUsageResult ItemEffect_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition) override
	{
		switch (ItemID){
		//....
		case 10:
		{
		Cast<AMyPlayer> SourceActor->HP += 50;	// Usage behavior here
		return ENaItemContainerUsageResult::Succeeded;	// Return succeeded because this item is usable
		}
		//....
	};
	//....
};

	In Item Effect Data Table:
		ID: 0000010,
		EffectClass: UMyEffect, // Browsed in editor here
		ConsumptionType: ConsumingOne, // Set here and consumption will be executed in container's using item
		IntParam: -1,	// ConsumingOne has no int param input, so it will not be applied
		TypeArray: {}	// ConsumingOne has no type array input, so it will not be applied
*/