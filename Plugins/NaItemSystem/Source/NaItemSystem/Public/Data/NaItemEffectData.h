#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../NaItemEffect.h"
#include "NaItemEffectData.generated.h"

// Data table row struct defining usage effect of item
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemEffectData : public FTableRowBase {

	GENERATED_BODY()

public:

	/* Usage effect function is called only when this value is true. When false, the usage effect function will be skipped.
	* Set false for avoiding unexpected usage or errors.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanUse = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UNaItemEffect> EffectClass = UNaItemEffect::StaticClass();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ENaItemUsageConsumptionType ConsumptionType = ENaItemUsageConsumptionType::IUCT_One;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int IntParam = -1;

public:


};