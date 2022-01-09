#include "Database/NaItemType.h"
#include "Kismet/KismetStringLibrary.h"
#include "NaUtility.h"

UDataTable * const FNaItemType::ItemTypeDataTable = LoadObject<UDataTable>(nullptr, TEXT("DataTable'/NaItemSystem/ItemType/ItemTypeDataTable.ItemTypeDataTable'"));

static FNaItemType DefaultType = FNaItemType();

bool FNaItemTypeDatabaseEntry::IsValidRowName(FName InRowName) {
	FString InStr = InRowName.ToString();
	if (InStr.Len() != 7)
		return false;
	if (InStr == TEXT("0000000"))
		return false;

	int i = 0;
	for (i = 0; i < 7; ++i) {
		if (((*InStr)[i] < (TCHAR)('0')) || ((*InStr)[i] > (TCHAR)('9')))
			return false;
	}
	return true;
}

FName FNaItemTypeDatabaseEntry::IntToRowName(int ID) {

	if (ID < 0 || ID > 9999999) {
		UE_LOG(LogNaItem, Warning, TEXT("NaItemTypeDatabaseEntry::ToRowName : invalid ID."));
		return TEXT("0000000");
	}

	TCHAR Str[8];
	Str[7] = '\0';
	for (int i = 6; i >= 0 ; --i) {
		Str[i] = ID % 10 + '0';
		ID /= 10;
	}
	ensureAlwaysMsgf(IsValidRowName(FName(Str)), TEXT("NaItemTypeDatabaseEntry::ToRowName Error: An invalid RowName is created."));
	return FName(Str);
}


int FNaItemTypeDatabaseEntry::RowNameToInt(FName InRowName) {

	if (!IsValidRowName(InRowName)) {
		UE_LOG(LogNaItem, Warning, TEXT("NaItemTypeDatabaseEntry::ToInt : invalid row name."));
		return 0;
	}
	return UKismetStringLibrary::Conv_StringToInt(InRowName.ToString());
}

FNaItemType::FNaItemType(int ItemID) {
	ID = ItemID;
	if (!IsValidID(ItemID)) {
		ID = 0;
		ItemID = 0;
	}
	TypeData = TSharedPtr<FNaItemTypeDatabaseEntry>(ItemTypeDataTable->FindRow<FNaItemTypeDatabaseEntry>(FNaItemTypeDatabaseEntry::IntToRowName(ItemID), TEXT("NaItemType")));
}