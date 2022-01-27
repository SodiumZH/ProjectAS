#include "Database/NaItemType.h"
#include "Kismet/KismetStringLibrary.h"
#include "NaUtility.h"


FNaItemTypeDatabaseEntry::FNaItemTypeDatabaseEntry(FString InName, int InStackingAmount, bool bInIsUnique, UDataTable* InUniqueDataTable, TSubclassOf<ANaItemEffect> InEffectClass) {
	Name = InName;
	MaxStackingAmount = InStackingAmount;
	bIsUnique = bInIsUnique;
	UniqueDataTable = InUniqueDataTable;
	EffectClass = InEffectClass;
}

FNaItemTypeDatabaseEntry::FNaItemTypeDatabaseEntry(const FNaItemTypeDatabaseEntry & CopyFrom){
	Name = CopyFrom.Name;
	MaxStackingAmount = CopyFrom.MaxStackingAmount;
	bIsUnique = CopyFrom.bIsUnique;
	UniqueDataTable = CopyFrom.UniqueDataTable;
	EffectClass = CopyFrom.EffectClass;
}

bool FNaItemTypeDatabaseEntry::IsValidRowName(FName InRowName) {
	FString InStr = InRowName.ToString();
	if (InStr.Len() != 7)
		return false;

	int i = 0;
	for (i = 0; i < 7; ++i) {
		if (((*InStr)[i] < (TCHAR)('0')) || ((*InStr)[i] > (TCHAR)('9')))
			return false;
	}
	return true;
}

#pragma optimize("" , off)
FName FNaItemTypeDatabaseEntry::IntToRowName(int ID) {

	if (ID < 0 || ID > 9999999) {
		UE_LOG(LogNaItem, Warning, TEXT("NaItemTypeDatabaseEntry::ToRowName : invalid ID."));
		return TEXT("0000000");
	}

	TCHAR Str[8];
	Str[7] = '\0';
	int i = 6;

	for (i = 6; i >= 0 ; --i) {
		Str[i] = ID % 10 + '0';
		ID /= 10;
	}

	ensureAlwaysMsgf(IsValidRowName(FName(Str)), TEXT("NaItemTypeDatabaseEntry::ToRowName Error: An invalid RowName is created."));
	return FName(Str);
}
#pragma optimize("" , on)

int FNaItemTypeDatabaseEntry::RowNameToInt(FName InRowName) {

	if (!IsValidRowName(InRowName)) {
		UE_LOG(LogNaItem, Warning, TEXT("NaItemTypeDatabaseEntry::ToInt : invalid row name."));
		return 0;
	}
	return UKismetStringLibrary::Conv_StringToInt(InRowName.ToString());
}

FNaItemType::FNaItemType(int InID, TSharedPtr<FNaItemTypeDatabaseEntry> InData) {
	if (!IsValidID(InID)) {
		UE_LOG(LogNaItem, Warning, TEXT("Making NaItemType: invalid ID."));
		ID = 0;
		TypeData = nullptr;
	}
	else if (!InData.IsValid()) {
		UE_LOG(LogNaItem, Warning, TEXT("Making NaItemType: Making from an invalid type data."));
		ID = InID;
		TypeData = nullptr;
	}
	else {
		ID = InID;
		TypeData = InData;
	}
}

void FNaItemType::CopyTypeData(FNaItemTypeDatabaseEntry & OutData) const {
	if (!TypeData.IsValid()) {
		UE_LOG(LogNaItem, Warning, TEXT("Copying NaItemType: Trying copying from invalid type. Return default."));
		OutData = FNaItemTypeDatabaseEntry();
		return;
	}
	OutData = *TypeData;
}

TSharedPtr<FNaItemTypeDatabaseEntry> FNaItemType::CopyTypeData() const {
	if (!TypeData.IsValid()) {
		UE_LOG(LogNaItem, Warning, TEXT("Copying NaItemType: Trying copying from invalid type. Return default."));
		return TSharedPtr<FNaItemTypeDatabaseEntry>(new FNaItemTypeDatabaseEntry());
	}
	return TSharedPtr<FNaItemTypeDatabaseEntry>(new FNaItemTypeDatabaseEntry(*TypeData));
}