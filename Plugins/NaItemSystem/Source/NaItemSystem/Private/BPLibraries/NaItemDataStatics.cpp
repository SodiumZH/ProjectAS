#include "BPLibraries/NaItemDataStatics.h"
#include "NaItemEntry.h"
#include "NaUtility.h"
#include "NaItemType.h"
#include "NaItemEffect.h"
#include "Engine/DataTable.h"
#include "NaPublicDependencies/NaPublicDependencies.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "BPLibraries/NaItemStatics.h"

TOptional<UNaItemType*> UNaItemDataStatics::GetItemTypeFromName(UObject* WorldContext, FName name){
	UNaGameModeItemSystemComponent * GMComp = UNaItemStatics::GetGameModeItemSystemComponent(WorldContext);
	if (!IsValid(GMComp)) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item display data failed: GameModeItemSystemComponent isn't correctly loaded to game mode."));
		return TOptional<UNaItemType*>();
	}
	if (!IsValid(GMComp->ItemTypeDataTable)) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item display data failed: item display data table is invalid. Set in GameModeItemSystemComponent."));
		return TOptional<UNaItemType*>();
	}
	UNaItemType** RetValPtr = GMComp->ItemTypeDataTable
		->FindRow<UNaItemType*>(name, TEXT("ItemDisplayDataTable"));
	return RetValPtr ? TOptional<UNaItemType*>(*RetValPtr) : TOptional<UNaItemType*>();
}


