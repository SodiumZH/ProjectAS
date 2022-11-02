#include "BPLibraries/NaItemDataStatics.h"
#include "NaItemEntry.h"
#include "NaUtility.h"
#include "Data/NaItemType.h"
#include "NaItemEffect.h"
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

FNaItemDisplayData UNaItemDataStatics::GetItemDisplayDataFromID(UObject* WorldContext, int ID){
	UNaGameModeItemSystemComponent * GMComp = UNaItemStatics::GetGameModeItemSystemComponent(WorldContext);
	if (!IsValid(GMComp)) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item display data failed: GameModeItemSystemComponent isn't correctly loaded to game mode."));
		return FNaItemDisplayData();
	}
	if (!IsValid(GMComp->ItemDisplayDataTable)) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item display data failed: item display data table is invalid. Set in GameModeItemSystemComponent."));
		return FNaItemDisplayData();
	}
	if (GMComp->ItemDisplayDataTable->RowStruct != FNaItemDisplayData::StaticStruct()) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item display data failed: item display data table row struct must be FNaItemDisplayData."));
		return FNaItemDisplayData();
	}
	FNaItemDisplayData* RetValPtr = GMComp->ItemDisplayDataTable->FindRow<FNaItemDisplayData>(FNaItemTypeData::IntToRowName(ID), TEXT("ItemDisplayDataTable"));
	return RetValPtr ? (*RetValPtr) : FNaItemDisplayData();
}

FNaItemEffectData UNaItemDataStatics::GetItemEffectDataFromID(UObject* WorldContext, int ID) {
	UNaGameModeItemSystemComponent * GMComp = UNaItemStatics::GetGameModeItemSystemComponent(WorldContext);
	if (!IsValid(GMComp)) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item effect data failed: GameModeItemSystemComponent isn't correctly loaded to game mode."));
		return FNaItemEffectData();
	}
	if (!IsValid(GMComp->ItemDisplayDataTable)) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item effect data failed: item display data table is invalid. Set in GameModeItemSystemComponent."));
		return FNaItemEffectData();
	}
	if (GMComp->ItemDisplayDataTable->RowStruct != FNaItemDisplayData::StaticStruct()) {
		UE_LOG(LogNaItem, Warning, TEXT("Get item effect data failed: item display data table row struct must be FNaItemEffectData."));
		return FNaItemEffectData();
	}
	FNaItemEffectData* RetValPtr = GMComp->ItemEffectDataTable->FindRow<FNaItemEffectData>(FNaItemTypeData::IntToRowName(ID), TEXT("ItemEffectDataTable"));
	return RetValPtr ? (*RetValPtr) : FNaItemEffectData();
}

FNaItemDescriptor UNaItemDataStatics::MakeDefaultDescriptor(int ID) {
	return FNaItemDescriptor(ID);
}

