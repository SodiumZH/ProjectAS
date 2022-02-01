// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "NaPublicDependencies/NaGameModeSubunitComponent.h"
#include "Data/NaItemDisplayData.h"
#include "Data/NaItemType.h"
#include "NaGameModeItemSystemComponent.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS(ClassGroup = (NaItemSystem), meta = (BlueprintSpawnableComponent))
class NAITEMSYSTEM_API UNaGameModeItemSystemComponent : public UNaGameModeSubunitComponent
{
	GENERATED_BODY()
	
public:

	// Basic item type data table. Warning: row struct of this data table must be FNaItemTypeData or subtype.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|DataTables")
	UDataTable * ItemTypeDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|DataTables")
	UDataTable * ItemDisplayDataTable;

public:

	/* Default type data struct for reply when reading a null data piece */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|Defaults")
	FNaItemTypeData DefaultTypeData = FNaItemTypeData();

	/* Default display data struct for reply when reading a null data piece */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|Defaults")
	FNaItemDisplayData DefaultDisplayData = FNaItemDisplayData();

	/* Default brush information for drawing item icons in UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|Defaults")
	FNaItemDefaultSlotSettings DefaultSlotSettings = FNaItemDefaultSlotSettings();

};
