#include "NaItemEntry.h"


/**** FNaItemDescriptor ****/

bool FNaItemDescriptor::IsEqual(const FNaItemDescriptor & Other) const {

	bool ThisIsUnique = (UniqueItemID >= 0 && IsValid(UniqueItemDatabase));
	bool OtherIsUnique = (Other.UniqueItemID >= 0 && IsValid(Other.UniqueItemDatabase));

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

bool FNaItemEntry::IsValid() {
	if (Amount <= 0) {
		UE_LOG(LogNaItem, Warning, TEXT("Invalid item entry: amount <= 0."));
		return false;
	}
	return true;
}



