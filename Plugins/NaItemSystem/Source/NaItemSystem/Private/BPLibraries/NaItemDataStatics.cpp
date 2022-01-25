#include "BPLibraries/NaItemDataStatics.h"
#include "NaItemEntry.h"
#include "Database/NaItemType.h"
#include "Effect/NaItemEffect.h"
#include "Engine/DataTable.h"

FNaItemTypeDatabaseEntry UNaItemDataStatics::DefaultTypeEntry:FNaItemTypeDatabaseEntry() {};

void UNaItemDataStatics::UNaItemDataStatics():UBlueprintFunctionLibrary() {
	UDataTable * TypeTable = FItemType::ItemTypeDataTable;
	if (IsValid(TypeTable)) {
		FNaItemTypeDatabaseEntry ZeroRow = TypeTable->FindRow<FNaItemTypeDatabaseEntry>(TEXT("0000000"), TEXT(""), false);
		if (!ZeroRow) {
			UE_LOG(LogNaItem, Display, TEXT("Item type data table missing default (zero) row. Has been automatically added."));
			TypeTable->AddRow(TEXT("0000000"), DefaultTypeEntry);
		}
	}
}

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