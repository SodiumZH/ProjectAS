#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaItemEffect.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemUsageReturn {

	GENERATED_BODY()

public:
	
	// Whether usage succeeded
	UPROPERTY(BlueprintReadWrite)
	bool bSucceeded = false;
	FNaItemUsageReturn SetSucceeded(bool value);

	// Amount of items consumed per usage
	UPROPERTY(BlueprintReadWrite)
	int ConsumedAmount = 0;
	FNaItemUsageReturn SetConsumedAmount(int value);

	// Usage failed, nothing happened
	static FNaItemUsageReturn Null();
	
	// Usage succeeded and an item was consumed
	static FNaItemUsageReturn Consumed();

	// Usage succeeded but not consumed
	static FNaItemUsageReturn UsedNoConsumption();

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
	//virtual FNaItemUsageReturn ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);

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
	FNaItemUsageReturn ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);
	virtual FNaItemUsageReturn ItemEffect_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);

public:

	FNaItemUsageReturn UseItem(UObject* WorldContext, int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition);

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