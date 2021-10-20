#include "NaItemManager.h"
#include "../NaUtility/NaUtility.h"

NAPACK_API const FNaItemSocket::EmptySocket(true, 0, TEXT("Empty"), 1, -1, NAME_None);

void UNaItemContainerComponent::InitializeContainer() {
	if (MaxSize > ITEM_CONTAINER_MAX_SIZE) {
		LogError("Size is larger than the defined maximum. Shrink the size or change the macro\"ITEM_CONTAINER_MAX_SIZE\" in NaPack/NaItemSystem/NaItemManager.h.");
		MaxSize = ITEM_CONTAINER_MAX_SIZE;
	}
	
	for (int i = 1; i <= MaxSize; ++i) {
		Container.Emplace(i, FNaItemSocket::EmptySocket);
	}
}

bool UNaItemContainerComponent::IsValidIndex(int Index) {
	return Index > 0 && Index <= MaxSize;
}

bool UNaItemContainerComponent::Resize(int NewSize, bool ForceShrink) {

	if (NewSize > ITEM_CONTAINER_MAX_SIZE) {
		LogError("Resize: New size is larger than the defined maximum. Shrink the size or change the macro\"ITEM_CONTAINER_MAX_SIZE\" in NaPack/NaItemSystem/NaItemManager.h.");
		NewSize = ITEM_CONTAINER_MAX_SIZE;
	}

	if (NewSize == MaxSize)
		return true;

	else if (NewSize > MaxSize) {
		for (int i = MaxSize + 1; i <= NewSize; ++i) {
			Container.Emplace(i, FNaItemSocket::EmptySocket);
		}
		MaxSize = NewSize;
		return true;
	}
	
	else {
		bool AllEmpty = true;
		for (int i = NewSize + 1; i <= MaxSize; ++i) {
			if (!Container[i].bIsEmpty) {
				AllEmpty = false;
				break;
			}
		}
		if (AllEmpty || ForceShrink) {
			for (i = NewSize + 1; i <= MaxSize; ++i) {
				Container.Remove(i);
			}
			MaxSize = NewSize;
			return true;
		}
		else return false;
	}
	checkNoEntry();
}

ENaItemContainerBrowseResult UNaItemContainerComponent::CheckItem(int Index) {
	if (Index <= 0 || Index > ITEM_CONTAINER_MAX_SIZE) {
		return ENaItemContainerBrowseResult::ICBR_InvID;
	}
	else if (Index > MaxSize) {
		return ENaItemContainerBrowseResult::ICBR_OOS;
	}
	else if (!Container[index].IsValidSocket()) {
		return ENaItemContainerBrowseResult::ICBR_InvVal;
	}
	else if (Container[index].bIsEmpty) {
		return ENaItemContainerBrowseResult::ICBR_Filled;
	}
	else return ENaItemContainerBrowseResult::ICBR_Empty;
}

ENaItemContainerBrowseResult UNaItemContainerComponent::GetItem(int Index, FNaItemSocket& OutItem) {
	if (Index <= 0 || Index > ITEM_CONTAINER_MAX_SIZE) {
		OutItem = FNaItemSocket::EmptySocket;
		return ENaItemContainerBrowseResult::ICBR_InvID;
	}
	else if (Index > MaxSize) {
		OutItem = FNaItemSocket::EmptySocket;
		return ENaItemContainerBrowseResult::ICBR_OOS;
	}
	else if (!Container[index].IsValidSocket()) {
		OutItem = FNaItemSocket::EmptySocket;
		return ENaItemContainerBrowseResult::ICBR_InvVal;
	}
	else if (Container[index].bIsEmpty) {
		return ENaItemContainerBrowseResult::ICBR_Filled;
	}
	else return ENaItemContainerBrowseResult::ICBR_Empty;
}

bool UNaItemContainerComponent::CheckContainer(TArray<int>& ErrorIndex) {
	bool AllCorrect = true;
	ErrorIndex.Empty();

	ENaItemContainerBrowseResult CheckResTemp;
	for (int i = 1; i <= MaxSize; ++i) {
		CheckResTemp = CheckItem(i);
		if (!(CheckResTemp(i) == ENaItemContainerBrowseResult::Filled || CheckResTemp(i) == ENaItemContainerBrowseResult::Empty)) {
			AllCorrect = false;
			ErrorIndex.Emplace(i);
		}
	}
	return AllCorrect;
}












