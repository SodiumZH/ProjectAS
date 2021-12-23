
#include "NaItemManager.h"
#include "NaUtility.h"
#include "Engine/DataTable.h"


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
		if ((UniqueItemID != Other.UniqueItemID) || (UniqueItemDatabase != Other.UniqueItemDatabase))
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




// Found item successfully
FNaItemContainerFindingReturn::FNaItemContainerFindingReturn(TSharedPtr<FNaItemEntry> InEntryPtr){
	EntryPtr = InEntryPtr;
	Result = ENaItemContainerFindingResult::ICFR_Filled;
}

// Not found
FNaItemContainerFindingReturn::FNaItemContainerFindingReturn(NotFoundType Type) {
	EntryPtr = nullptr;
	switch (Type)
	{
	case FNaItemContainerFindingReturn::NotFoundType::Empty:
		Result = ENaItemContainerFindingResult::ICFR_Empty;
		break;
	case FNaItemContainerFindingReturn::NotFoundType::OOS:
		Result = ENaItemContainerFindingResult::ICFR_OOS;
		UE_LOG(LogNaItem, Log, TEXT("Item finding: out of container size."));
		break;
	case FNaItemContainerFindingReturn::NotFoundType::InvID:
		Result = ENaItemContainerFindingResult::ICFR_InvID;
		UE_LOG(LogNaItem, Warning, TEXT("Item finding: wrong input index (negative or larger than ITEM_CONTAINER_MAX_SIZE)."));
		break;
	case FNaItemContainerFindingReturn::NotFoundType::InvVal:
		Result = ENaItemContainerFindingResult::ICFR_InvVal;
		UE_LOG(LogNaItem, Error, TEXT("Item finding: wrong item entry detected."));
		break;
	case FNaItemContainerFindingReturn::NotFoundType::Error:
		UE_LOG(LogNaItem, Error, TEXT("Item finding: unknown error detected."));
		Result = ENaItemContainerFindingResult::ICFR_Error;
		break;
	default:
		checkNoEntry();
	}
}

FNaItemContainerFindingReturn FNaItemContainer::Find(int Index) {
	
	/* Out of size check */
	if (Index < 0 || Index > ITEM_CONTAINER_MAX_SIZE)
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::InvID);
	
	if (Index >= Size)
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::OOS);
	
	// This is a ptr to a shared ptr
	TSharedPtr<FNaItemEntry>* ValPtrPtr = Content.Find(Index);

	if (!ValPtrPtr)	// Entry not created = empty
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::Empty);
	if (!(*ValPtrPtr).IsValid())	// Entry is empty/null
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::Empty);
	if (!(*ValPtrPtr)->IsValid())	// Entry is invalid
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::InvVal);

	// Succeeded
	return FNaItemContainerFindingReturn(*ValPtrPtr);
}

bool FNaItemContainer::FindItem(int TypeIndex, TArray<int>& Positions, bool bIncludeUniquified) {
	
	Positions.Empty();
	int i = 0;
	TSharedPtr<FNaItemEntry> Temp;
	for (i = 0; i < Size; ++i) {
		Temp = Find(i).EntryPtr;
		if (!Temp.IsValid())
			continue;
		if (Temp->TypeDescriptor.ItemTypeID == TypeIndex) {
			if (!Temp->TypeDescriptor.bIsUniquified || bIncludeUniquified)
				Positions.Add(i);
		}
	}

	return Positions.Num() != 0;

}

bool FNaItemContainer::FindItemExplicit(const FNaItemDescriptor & Type, TArray<int>& Positions) {
	Positions.Empty();

	int i = 0;
	TSharedPtr<FNaItemEntry> Temp;
	for (i = 0; i < Size; ++i) {
		Temp = Find(i).EntryPtr;
		if (!Temp.IsValid())
			continue;
		if (Temp->TypeDescriptor == Type) {
			Positions.Add(i);
		}
	}

	return Positions.Num() != 0;
}

bool FNaItemContainer::AddEntry(int Position, const FNaItemEntry & Entry, bool bForce) {

	// Check OOS	
	if (!IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("Add entry: out of size."));
		return false;
	}

	TSharedPtr<FNaItemEntry>* TempPtr = Content.Find(Position);

	// Case if no index
	if (!TempPtr) {
		Content.Add(Position, TSharedPtr<FNaItemEntry>(new FNaItemEntry(Entry)));
		return true;
	}
	// Case if index is created but empty (pointed to null)
	if (!(*TempPtr).IsValid()){
		(*TempPtr) = TSharedPtr<FNaItemEntry>(new FNaItemEntry(Entry));	// Redirect the ptr to a created entry object
		return true;
	}

	// Case if occupied, but force add
	if (bForce) {
		UE_LOG(LogNaItem, Log, TEXT("Add entry: an entry is replaced due to bForce."));
		(*TempPtr) = TSharedPtr<FNaItemEntry>(new FNaItemEntry(Entry));
		return true;
	}

	// Occupied, don't add
	return false;
}

void FNaItemContainer::RemoveEntry(int Position) {

	// Check OOS	
	if (!IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("Remove entry: out of size."));
		return;
	}

	TSharedPtr<FNaItemEntry>* TempPtr = Content.Find(Position);
	
	// Already empty
	if (!TempPtr || !(*TempPtr).IsValid())
		return;

	// Occupied
	else (*TempPtr) = nullptr;

}

bool FNaItemContainer::MoveEntry(int From, int To, bool bForce) {

	// Check OOS	
	if (!IsInSize(From) || !IsInSize(To)) {
		UE_LOG(LogNaItem, Warning, TEXT("Move entry: out of size."));
		return false;
	}

	TSharedPtr<FNaItemEntry>* FromPtr = Content.Find(From);
	TSharedPtr<FNaItemEntry>* ToPtr = Content.Find(To);
	
	// Case when from is empty
	if (!FromPtr || !(*FromPtr).IsValid()) {
		UE_LOG(LogNaItem, Warning, TEXT("Move entry: trying to move empty entry. Use RemoveEntry() to empty a position."));
		return false;
	}
	// Case when to is occupied
	if (ToPtr && (*ToPtr).IsValid()) {
		if (bForce) {
			(*ToPtr) = (*FromPtr);
			(*FromPtr) = nullptr;
			UE_LOG(LogNaItem, Log, TEXT("Move entry: an entry is overwritten due to bForce."));
			return true;
		}
		else {
			UE_LOG(LogNaItem, Log, TEXT("Move entry: failed: trying to move into an occupied position."));
			return false;
		}
	}
	// Ideal case
	else {
		(*ToPtr) = (*FromPtr);
		(*FromPtr) = nullptr;
		return true;
	}
}

void FNaItemContainer::SwapEntry(int P1, int P2) {

	// Check OOS
	if (!IsInSize(P1) || !IsInSize(P2)) {
		UE_LOG(LogNaItem, Warning, TEXT("Swap entry: out of size."));
		return;
	}

	TSharedPtr<FNaItemEntry>* P1Ptr = Content.Find(P1);
	TSharedPtr<FNaItemEntry>* P2Ptr = Content.Find(P2);

	if (!P1Ptr && !P2Ptr)
		return;
	else if (!P1Ptr) {
		(*P1Ptr) = (*P2Ptr);
		(*P2Ptr) = nullptr;
		return;
	}
	else if (!P2Ptr) {
		(*P2Ptr) = (*P1Ptr);
		(*P1Ptr) = nullptr;
		return;
	}
	else {
		TSharedPtr<FNaItemEntry> Temp = *P1Ptr;
		(*P1Ptr) = (*P2Ptr);
		(*P2Ptr) = Temp;
	}

}