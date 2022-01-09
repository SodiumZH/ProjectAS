#include "BPLibraries/NaItemDataStatics.h"
#include "Database/NaItemType.h"

void UNaItemDataStatics::BreakItemType(const FNaItemType & InType, int & ID, FString & StrName, int & MaxStackingAmount, bool & IsUnique, UDataTable*& UniqueDataTable, TSubclassOf<class AItemEffect>& EffectClass) {
	ID = InType.GetID();

}

FNaItemType UNaItemDataStatics::GetItemTypeFromID(int ID) {

}