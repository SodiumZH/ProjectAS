// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "NaPublicDependencies/NaGameModeSubunitComponent.h"
#include "NaGameModeItemSystemComponent.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class NAITEMSYSTEM_API UNaGameModeItemSystemComponent : public UNaGameModeSubunitComponent
{
	GENERATED_BODY()
	
public:

	// Basic item type data table. Warning: row name of this data table must be FNaItemTypeDatabaseEntry.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSystem|DataTables")
	UDataTable * ItemTypeDataTable;

public:



};
