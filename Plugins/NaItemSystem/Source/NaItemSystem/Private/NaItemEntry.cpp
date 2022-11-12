#include "NaItemEntry.h"
#include "BPLibraries/NaItemDataStatics.h"

/**** FNaItemDescriptor ****/

bool FNaItemDescriptor::IsEqual(const FNaItemDescriptor & Other) const {

	bool ThisIsUnique = (UniqueItemID >= 0);
	bool OtherIsUnique = (Other.UniqueItemID >= 0);

	// Compare basic type
	if (ItemTypeID != Other.ItemTypeID)
		return false;

	// Compare unique items
	if (ThisIsUnique != OtherIsUnique)
		return false;
	if (ThisIsUnique)
		if (UniqueItemID != Other.UniqueItemID)
			return false;

	// Compare uniqufications
	if (bIsUniquified != Other.bIsUniquified)
		return false;
	if (bIsUniquified)
		if (UniqueName != Other.UniqueName)
			return false;

	return true;

}

bool FNaItemDescriptor::operator==(const FNaItemDescriptor & Other) const {

	return IsEqual(Other);

}

bool FNaItemDescriptor::operator!=(const FNaItemDescriptor & Other) const {

	return !IsEqual(Other);

}

/**** FNaItemEntry ****/



FNaItemEntry::FNaItemEntry(const FNaItemEntry & CopyFrom) {

	TypeDescriptor = CopyFrom.TypeDescriptor;
	Amount = CopyFrom.Amount;

}

bool FNaItemEntry::IsValid() const {
	if (Amount <= 0) {
		UE_LOG(LogNaItem, Warning, TEXT("Invalid item entry: amount <= 0."));
		return false;
	}
	return true;
}

FNaItemEntry FNaItemEntry::SetAmount(int val) {
	check(val >= 0);
	Amount = val;
	return *this;
}

FNaItemEntry FNaItemEntry::AddToAmount(int val) {
	check(val >= 0);
	Amount += val;
	return *this; 
}

FNaItemEntry FNaItemEntry::RemoveFromAmount(int val) {
	check(val >= 0);
	checkf(val <= Amount, TEXT("FNaItemEntry error: negative amount after removing from amount."));
	Amount -= val;
	return *this;
}

int FNaItemEntry::GetItemID() const {
	return TypeDescriptor.ItemTypeID;
}

int FNaItemEntry::GetMaxStackingAmount(UObject* WorldContext) const {
	return UNaItemDataStatics::GetItemTypeFromID(WorldContext, GetItemID()).GetTypeData().MaxStackingAmount;
}


