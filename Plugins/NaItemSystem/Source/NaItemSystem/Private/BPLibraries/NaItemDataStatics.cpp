#include "BPLibraries/NaItemDataStatics.h"
#include "NaItemEntry.h"
#include "NaUtility.h"
#include "Data/NaItemType.h"
#include "Actors/NaItemEffect.h"
#include "Engine/DataTable.h"
#include "NaPublicDependencies/NaPublicDependencies.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "BPLibraries/NaItemStatics.h"

FName UNaItemDataStatics::ToItemRowName(int InIndex) {
	return FNaItemTypeData::IntToRowName(InIndex);
}

int UNaItemDataStatics::ToIndex(FName InItemRowName) {
	return FNaItemTypeData::RowNameToInt(InItemRowName);
}


void UNaItemDataStatics::BreakItemType(const FNaItemType & InType, int & ID, FString & StrName, int & MaxStackingAmount, bool & bIsUnique, UDataTable*& UniqueDataTable, FName & RowName) {
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
	RowName = FNaItemTypeData::IntToRowName(ID);
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
	if (GMComp->ItemTypeDataTable->RowStruct != FNaItemTypeData::StaticStruct()) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item type failed: item type data table row struct must be FNaItemTypeData."));
		return FNaItemType();
	}
	return FNaItemType(ID, TSharedPtr<FNaItemTypeData>(GMComp->ItemTypeDataTable->FindRow<FNaItemTypeData>(FNaItemTypeData::IntToRowName(ID), TEXT("ItemTypeDataTable"))));
}

FNaItemDescriptor UNaItemDataStatics::MakeDefaultDescriptor(int ID) {
	return FNaItemDescriptor(ID);
}

bool UNaItemDataStatics::AddItemType(int ID, const FNaItemTypeDatabaseEntry & Data, bool bAllowOverwrite = true) {
	FName RowName = FNaItemTypeDatabaseEntry::IntToRowName(ID);
	if (bAllowOverwrite && FNaItemType::ItemTypeDataTable->FindRow(RowName, TEXT(""), false) == nullptr) {
		FNaItemType::ItemTypeDataTable->AddRow(RowName, Data);
		return true;
	}
	else {
		UE_LOG(LogNaItem, Display, TEXT("Add item type failed: row ID is existing."));
		return false;
	}

}