#include "BPLibraries/NaItemDataStatics.h"
#include "NaItemType.h"
#include "Data/NaItemTableRow.h"
#include "Engine/DataTable.h"
#include "NaPublicDependencies/NaPublicDependencies.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "BPLibraries/NaItemStatics.h"

TOptional<UNaItemType*> UNaItemDataStatics::GetItemTypeFromName(UObject* WorldContext, FName name){
	UNaGameModeItemSystemComponent* GMComp = UNaItemStatics::GetGameModeItemSystemComponent(WorldContext);
	if (!IsValid(GMComp)) {
		UE_LOG(LogNaItem, Warning, TEXT("GetItemTypeFromName failed: GameModeItemSystemComponent isn't correctly loaded to game mode."));
		return TOptional<UNaItemType*>();
	}
	if (!IsValid(GMComp->ItemTypeDataTable)) {
		UE_LOG(LogNaItem, Warning, TEXT("GetItemTypeFromName failed: item type data table is invalid. Set in GameModeItemSystemComponent."));
		return TOptional<UNaItemType*>();
	}
	FNaItemTableRow* Row = GMComp->ItemTypeDataTable->FindRow<FNaItemTableRow>(name, TEXT("GetItemTypeFromName"));
	return Row ? TOptional<UNaItemType*>(Row->Type) : TOptional<UNaItemType*>();
}

