
#include "BPLibraries/NaItemStatics.h"
#include "NaItemEntry.h"

void UNaItemStatics::FindEntryFromIndex(const FNaItemContainer & Target, int Position, FNaItemContainerFindingReturn & ReturnType, TSharedPtr<FNaItemEntry> & Result) {
	auto Return = Target.Find(Position);
	ReturnType = Return.Result;
	Result = Return.EntryPtr;
}

bool UNaItemStatics::IsValidItemEntry(const FNaItemEntry & ItemEntry) {
	return ItemEntry.IsValid();
}

bool UNaItemStatics::ResizeContainer(TSharedPtr<FNaItemContainer> Target, int NewSize, bool bForce) {
	if (!Target.IsValid())
		return false;
	return Target->Resize(NewSize, bForce);
}

