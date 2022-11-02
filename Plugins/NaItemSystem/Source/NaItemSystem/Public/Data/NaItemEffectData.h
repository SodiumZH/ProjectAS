#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NaItemEffectData.generated.h"

// Data table row struct defining usage effect of item
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemEffectData : public FTableRowBase {

	GENERATED_BODY()

public:

	TSubclassOf<class UNaItemEffect> EffectClass;

public:


};