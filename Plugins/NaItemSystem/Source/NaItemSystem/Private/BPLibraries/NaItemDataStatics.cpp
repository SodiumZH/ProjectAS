#include "BPLibraries/NaItemDataStatics.h"
#include "NaItemEntry.h"
#include "NaUtility.h"
#include "Database/NaItemType.h"
#include "Effect/NaItemEffect.h"
#include "Engine/DataTable.h"
#include "NaPublicDependencies/NaPublicDependencies.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "BPLibraries/NaItemStatics.h"




void UNaItemDataStatics::BreakItemType(const FNaItemType & InType, int & ID, FString & StrName, int & MaxStackingAmount, bool & bIsUnique, UDataTable*& UniqueDataTable, TSubclassOf<ANaItemEffect>& EffectClass, FName & RowName) {
	if (!InType.IsValid()) {
		UE_LOG(LogNaItem, Error, TEXT("Break Item Type: invalid input."));
		return;
	}
	ID = InType.GetID();
	const auto & Data = InType.GetTypeData();
	StrName = Data.Name;
	MaxStackingAmount = Data.MaxStackingAmount;
	bIsUnique = Data.bIsUnique;
	UniqueDataTable = Data.UniqueDataTable;
	EffectClass = Data.EffectClass;
	RowName = FNaItemTypeDatabaseEntry::IntToRowName(ID);
}

FNaItemType UNaItemDataStatics::GetItemTypeFromID(UObject* WorldContext, int ID) {
	UNaGameModeItemSystemComponent * GMComp = UNaItemStatics::GetGameModeItemSystemComponent(WorldContext);
	if (!IsValid(GMComp)) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item type failed: GameModeItemSystemComponent isn't correctly loaded to game mode."));
		return FNaItemType();
	}
	if (!IsValid(GMComp->ItemTypeDataTable)) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item type failed: item type data table is invalid. Set in GameModeItemSystemComponent."));
		return FNaItemType();
	}
	if (GMComp->ItemTypeDataTable->RowStruct != FNaItemTypeDatabaseEntry::StaticStruct()) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item type failed: item type data table row struct must be FNaItemTypeDatabaseEntry."));
		return FNaItemType();
	}
	return FNaItemType(ID, TSharedPtr<FNaItemTypeDatabaseEntry>(GMComp->ItemTypeDataTable->FindRow<FNaItemTypeDatabaseEntry>(FNaItemTypeDatabaseEntry::IntToRowName(ID), TEXT("ItemTypeDataTable"))));
}

FNaItemDescriptor UNaItemDataStatics::MakeDefaultDescriptor(int ID) {
	return FNaItemDescriptor(ID);
}