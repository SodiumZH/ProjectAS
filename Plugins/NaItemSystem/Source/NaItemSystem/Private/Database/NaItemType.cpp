#include "Database/NaItemType.h"
#include "Kismet/KismetStringLibrary.h"
#include "NaUtility.h"

FString FNaItemType::ItemTypeDataTablePath = TEXT("/NaItemSystem/ItemType/ItemTypeDataTable.ItemTypeDataTable");

UDataTable * FNaItemType::ItemTypeDataTable = LoadObject<UDataTable>(nullptr, *(TEXT("DataTable'") + FNaItemType::ItemTypeDataTablePath + TEXT("'")));

static FNaItemType DefaultType = FNaItemType();

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

FNaItemType::FNaItemType(int ItemID) {
	ID = ItemID;
	// Invalid ID: return default (ID==0)
	if (!IsValidID(ItemID)) {
		ID = 0;
		ItemID = 0;
	}

	// If data table is not correctly loaded, try reloading 
	if (!IsValid(ItemTypeDataTable)) {
		ReloadTypeDataTable();
	}
	// Both cases when initially correctly loaded or reloaded successfully
	if(IsValid(ItemTypeDataTable))
		TypeData = TSharedPtr<FNaItemTypeDatabaseEntry>(ItemTypeDataTable->FindRow<FNaItemTypeDatabaseEntry>(FNaItemTypeDatabaseEntry::IntToRowName(ItemID), TEXT("NaItemType")));
	// If reloaded but still failed, return null
	else {
		UE_LOG(LogNaItem, Error, TEXT("Item type data table not found. The data table directory should be \"%s\"."), *FNaItemType::ItemTypeDataTablePath);
		ID = 0;
		ItemID = 0;
		TypeData = nullptr;
	}
}

bool FNaItemType::RefindDataTable() {
	if (IsValid(ItemTypeDataTable)) {
		UE_LOG(LogNaItem, Display, TEXT("Type data table refinding: type data table is already valid. Overwritten."));
	}
	ItemTypeDataTable = LoadObject<UDataTable>(nullptr, *(TEXT("DataTable'") + FNaItemType::ItemTypeDataTablePath + TEXT("'")));
	if (!IsValid(ItemTypeDataTable)) {
		UE_LOG(LogNaItem, Display, TEXT("Type data table refinding: Not found. Current type data table path is \"%s\"."), *ItemTypeDataTablePath);
		return false;
	}
	else UE_LOG(LogNaItem, Display, TEXT("Type data table refinding: type data table refound."));
	return true;
	
}