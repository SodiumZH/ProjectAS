#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaItemEffect.generated.h"

class AActor;

USTRUCT()
struct NAITEMSYSTEM_API FNaItemUsageReturn {

	GENERATED_BODY()

public:
	
	// Whether usage succeeded
	UPROPERTY(BlueprintReadOnly)
	bool bSucceeded = false;
	FNaItemUsageReturn bSucceeded(bool value);

	// Amount of items consumed per usage
	UPROPERTY(BlueprintReadOnly)
	int ConsumedAmount = 0;
	FNaItemUsageReturn ConsumedAmount(int value);

	// Usage failed, nothing happened
	static FNaItemUsageReturn Null();
	
	// Usage succeeded and an item was consumed
	static FNaItemUsageReturn Consumed();

	// Usage succeeded but not consumed
	static FNaItemUsageReturn UsedNoConsumption();

};

/* Effect class when using items */
UCLASS(Blueprintable, BlueprintType)
class NAITEMSYSTEM_API UNaItemEffect :public UObject {

	GENERATED_BODY()

public:

	virtual bool CanUseItem(int ItemID, AActor* SourceActor, AActor* TargetActor) const;

	// Item effect defined in cpp.
	// @Param ItemID Item ID to be used.
	// @Param SourceActor Actor which uses the item.
	// @Param TargetActor Actor as target of the item.
	// @Param ItemPosition If the item comes from an item container, its position in the container. -1 means disabled.
	// @ReturnValue Whether the usage succeeded.
	virtual FNaItemUsageReturn ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);

	// If true, the effect will use blueprint function override (i.e. use ItemEffectBP() instead of ItemEffect()).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemEffect")
	bool bUseBlueprintOverride = false;

	// Item effect defined in BP.
	// @Param ItemID Item ID to be used.
	// @Param SourceActor Actor which uses the item.
	// @Param TargetActor Actor as target of the item.
	// @Param ItemPosition If the item comes from an item container, its position in the container. -1 means disabled.
	// @ReturnValue Whether the usage succeeded.
	UFUNCTION(BlueprintNativeEvent, DisplayName = "ItemEffectBlueprintOverride", Category = "NaItemSystem|ItemEffect")
	FNaItemUsageReturn ItemEffectBP(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);
	FNaItemUsageReturn ItemEffectBP_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor, int ItemPosition = -1);

public:

	// Use an item, taking real effects
	FNaItemUsageReturn UseItem(int ItemID, AActor* SourceActor, AActor* TargetActor);

};

