#include "NaItemManager.h"
#include "NaUtility.h"

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