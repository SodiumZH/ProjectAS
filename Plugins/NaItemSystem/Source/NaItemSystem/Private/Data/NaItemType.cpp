#include "Data/NaItemType.h"
#include "Kismet/KismetStringLibrary.h"
#include "NaUtility.h"


FNaItemTypeData::FNaItemTypeData(FString InName, int InStackingAmount, bool bInIsUnique, UDataTable* InUniqueDataTable) {
	Name = InName;
	MaxStackingAmount = InStackingAmount;
	bIsUnique = bInIsUnique;
	UniqueDataTable = InUniqueDataTable;
}

FNaItemTypeData::FNaItemTypeData(const FNaItemTypeData & CopyFrom){
	Name = CopyFrom.Name;
	MaxStackingAmount = CopyFrom.MaxStackingAmount;
	bIsUnique = CopyFrom.bIsUnique;
	UniqueDataTable = CopyFrom.UniqueDataTable;
}

bool FNaItemTypeData::IsValidRowName(FName InRowName) {
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
FName FNaItemTypeData::IntToRowName(int ID) {

	if (ID < 0 || ID > 9999999) {
		UE_LOG(LogNaItem, Warning, TEXT("NaItemTypeData::ToRowName : invalid ID."));
		return TEXT("0000000");
	}

	TCHAR Str[8];
	Str[7] = '\0';
	int i = 6;

	for (i = 6; i >= 0 ; --i) {
		Str[i] = ID % 10 + '0';
		ID /= 10;
	}

	ensureAlwaysMsgf(IsValidRowName(FName(Str)), TEXT("NaItemTypeData::ToRowName Error: An invalid RowName is created."));
	return FName(Str);
}
#pragma optimize("" , on)

int FNaItemTypeData::RowNameToInt(FName InRowName) {

	if (!IsValidRowName(InRowName)) {
		UE_LOG(LogNaItem, Warning, TEXT("NaItemTypeData::ToInt : invalid row name."));
		return 0;
	}
	return UKismetStringLibrary::Conv_StringToInt(InRowName.ToString());
}

FNaItemType::FNaItemType(int InID, FNaItemTypeData* InData) {
	if (!IsValidID(InID)) {
		UE_LOG(LogNaItem, Warning, TEXT("Making NaItemType: invalid ID."));
		ID = 0;
		TypeData = nullptr;
	}
	else if (!InData) {
		UE_LOG(LogNaItem, Warning, TEXT("Making NaItemType: Making from an invalid type data."));
		ID = InID;
		TypeData = nullptr;
	}
	else {
		ID = InID;
		TypeData = InData;
	}
}

void FNaItemType::CopyTypeData(FNaItemTypeData & OutData) const {
	if (!TypeData) {
		UE_LOG(LogNaItem, Warning, TEXT("Copying NaItemType: Trying copying from invalid type. Return default."));
		OutData = FNaItemTypeData();
		return;
	}
	OutData = *TypeData;
}

FNaItemTypeData FNaItemType::CopyTypeData() const {
	if (!TypeData) {
		UE_LOG(LogNaItem, Warning, TEXT("Copying NaItemType: Trying copying from invalid type. Return default."));
		return FNaItemTypeData();
	}
	return *TypeData;
}