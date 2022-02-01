#pragma once

#include "Engine/DataTable.h"
#include "NaItemDisplayData.generated.h"

/**
* Default settings for item display. This struct exists in NaGameModeItemSystemComponent.s
*/
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemDefaultSlotSettings {

	GENERATED_BODY()

public:

	/* Default slot size (in pixel). */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D SlotSize = FVector2D(32.f, 32.f);

	/* Base icon for empty slots */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconEmpty = nullptr;

	/* Border icon for empty and occupied slots, stacking above the base icon */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconBorder = nullptr;

	/* Base icon for disabled slot */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconDisabled = nullptr;

	/* Border icon for disabled slots, stacking above the base icon */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconBorderDisabled = nullptr;

	/* Additive image for pointed item slot (stack above base and border) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconPointed = nullptr;

	/* Additive image for selected item slot (stack above all other icons) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject* IconSelected = nullptr;

	/* If true, the disabled item slots will enable to be pointed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanPointAtDisabled = false;

	/* If true, the disabled item slots will enable to be pointed */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanSelectDisabled = false;

	FNaItemDefaultSlotSettings() {};
};


/**
* This is the item data applied by UI.
*
*/
USTRUCT(BlueprintType)
struct NAITEMSYSTEM_API FNaItemDisplayData : public FTableRowBase {

	GENERATED_BODY()

public:

	/* Image applied to display an item in container UI. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UObject * BrushImage = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DefaultDescription = TEXT("This is an item.");
	



};
