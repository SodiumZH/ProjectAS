#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../NaItemType.h"
#include "NaItemTableRow.generated.h"

USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemTableRow : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Natrium|Item System")
	UNaItemType* Type;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Natrium|Item System")
	FNaItemProperties Properties;
	
};
