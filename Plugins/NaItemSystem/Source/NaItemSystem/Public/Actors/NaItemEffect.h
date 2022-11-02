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
	static FNaItemUsageReturn Null;
	
	// Usage succeeded and an item was consumed
	static FNaItemUsageReturn Consumed;

	// Usage succeeded but not consumed
	static FNaItemUsageReturn UsedNoConsumption;

};

UCLASS()
class NAITEMSYSTEM_API UNaItemEffect :public UObject {

	GENERATED_BODY()

public:

	// Item effect defined in cpp.
	// @Param ItemID Item ID to be used.
	// @Param SourceActor Actor which uses the item.
	// @Param TargetActor Actor as target of the item.
	// @ReturnValue Whether the usage succeeded.
	virtual bool ItemEffect(int ItemID, AActor* SourceActor, AActor* TargetActor) {};

	UFUNCTION(BlueprintNativeEvent, Category = "NaItemSystem|ItemEffect")
	bool ItemEffectBP(int ItemID, AActor* SourceActor, AActor* TargetActor);
	bool ItemEffectBP_Implementation(int ItemID, AActor* SourceActor, AActor* TargetActor);

};