#include "BPLibraries/NaItemDataStatics.h"
#include "NaItemEntry.h"
#include "Database/NaItemType.h"
#include "Effect/NaItemEffect.h"


void UNaItemDataStatics::BreakItemType(const FNaItemType & InType, int & ID, FString & StrName, int & MaxStackingAmount, bool & bIsUnique, UDataTable*& UniqueDataTable, TSubclassOf<ANaItemEffect>& EffectClass, FName & RowName) {
	ID = InType.GetID();
	const auto & Data = InType.GetTypeData();
	StrName = Data.Name;
	MaxStackingAmount = Data.MaxStackingAmount;
	bIsUnique = Data.bIsUnique;
	UniqueDataTable = Data.UniqueDataTable;
	EffectClass = Data.EffectClass;
	RowName = FNaItemTypeDatabaseEntry::IntToRowName(ID);
}

FNaItemType UNaItemDataStatics::GetItemTypeFromID(int ID) {
	return FNaItemType(ID);
}

FNaItemDescriptor UNaItemDataStatics::MakeDefaultDescriptor(int ID) {
	return FNaItemDescriptor(ID);
}