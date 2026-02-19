#pragma once

#include "Engine/DataTable.h"
#include "Styling/SlateTypes.h"
#include "NaItemDisplayData.generated.h"



/**
* This is the item data applied by UI.
*
*/
USTRUCT(BlueprintType)
struct NAINVENTORY_API FNaItemDisplayData : public FTableRowBase {

	GENERATED_BODY()

public:

	/* Image applied to display an item in container UI. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowedClasses = "/Script/Engine.Texture, /Script/Engine.MaterialInterface, /Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	UObject* BrushImage = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString DefaultDescription = TEXT("This is an item.");
	



};
