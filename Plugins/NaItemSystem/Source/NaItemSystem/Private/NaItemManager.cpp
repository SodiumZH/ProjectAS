
#include "NaItemManager.h"
#include "NaUtility.h"
#include "Engine/DataTable.h"


bool FNaItemDescriptor::IsEqual(const FNaItemDescriptor & Other) {

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

bool FNaItemDescriptor::operator==(const FNaItemDescriptor & Other) {
	
	return IsEqual(Other);

}

bool FNaItemDescriptor::operator!=(const FNaItemDescriptor & Other) {

	return !IsEqual(Other);

}




const FNaItemEntry FNaItemEntry::EmptySocket(true, 0, TEXT("Empty"), 1, -1, NAME_None, false, TEXT(""));


// Found item successfully
FNaItemContainerFindingReturn::FNaItemContainerFindingReturn(TWeakPtr<FNaItemEntry> InEntryPtr){
	EntryPtr = InEntryPtr;
	Result = ENaItemContainerFindingResult::ICBR_Filled;
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
		break;
	case FNaItemContainerFindingReturn::NotFoundType::InvID:
		Result = ENaItemContainerFindingResult::ICFR_InvID;
		LogWarning("Item finding: wrong input index (negative or larger than ITEM_CONTAINER_MAX_SIZE).");
		break;
	case FNaItemContainerFindingReturn::NotFoundType::InvVal:
		Result = ENaItemContainerFindingResult::ICFR_InvVal;
		LogError("Item finding: wrong item entry detected.");
		break;
	case FNaItemContainerFindingReturn::NotFoundType::Error:
		LogError("Item finding: unknown error detected.");
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
	
	FNaItemEntry* ValPtr = Content.Find(Index);


	if (!ValPtr)
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::Empty);
	if (!ValPtr->IsValid())
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::InvID);
	if (ValPtr->bIsEmpty || ValPtr->Amount <= 0)
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::Empty);
	
	// Succeeded
	return FNaItemContainerFindingReturn(ValPtr);
}

bool FNaItemContainer::FindItem(int TypeIndex, TArray<int>& Positions, bool bIncludeUniquified) {
	
	Positions.Empty();

	for (auto & elem : Content) {
		if (elem.Value.TypeDescriptor.ItemTypeID == TypeIndex)
			if (bIncludeUniquified || (!elem.Value.TypeDescriptor.bIsUniquified))
				Positions.Add(elem.Key);
	}

	return Positions.Num() != 0;

}