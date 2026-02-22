// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "NaPublicDependencies/NaGameModeSubunitComponent.h"
#include "Data/NaItemTableRow.h"
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

	
public:
};
