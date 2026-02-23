#pragma once

#include "CoreMinimal.h"
#include "NaItemProperties.generated.h"

class UTexture2D;

USTRUCT(BlueprintType, Category = "Natrium|Item System")
struct NAITEMSYSTEM_API FNaItemProperties
{
	
	GENERATED_BODY()
	
public:
	
	/** Max stack size (like Minecraft's getMaxStackSize()) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Natrium|Item System", meta = (ClampMin = "1", ClampMax = "999"))
	int32 MaxStackSize;

	/** Display name for UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Natrium|Item System")
	FText DisplayName;

	/** Item icon for UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Natrium|Item System")
	UTexture2D* ItemIcon;

	/** Item rarity/quality */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Natrium|Item System")
	FLinearColor RarityColor;

	/** Tooltip description */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Natrium|Item System", meta = (MultiLine = "true"))
	FText Description;

};
