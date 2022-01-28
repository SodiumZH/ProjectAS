#pragma once

#include "Engine/DataTable.h"
#include "NaItemDisplayData.generated.h"

USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemDisplayData : public FTableRowBase {

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject * BrushImage = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DefaultDescription = TEXT("This is an item.");

};