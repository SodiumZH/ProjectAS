
#include "BPLibraries/NaItemStatics.h"
#include "NaItemEntry.h"
#include "Database/NaItemType.h"
#include "NaItemContainer.h"

const FNaItemDescriptor UNaItemStatics::EmptyItemDescriptor = FNaItemDescriptor();

const FNaItemEntry UNaItemStatics::EmptyItemEntry = FNaItemEntry();

bool UNaItemStatics::NotEmpty_Desc(const FNaItemDescriptor & Test) {
	return Test.ItemTypeID > 0;
}

bool UNaItemStatics::NotEmpty_Entry(const FNaItemEntry & Test) {
	return Test.TypeDescriptor.ItemTypeID > 0;
}

void UNaItemStatics::GetEmptyItemDescriptor(FNaItemDescriptor & EmptyDescriptor) {
	EmptyDescriptor = EmptyItemDescriptor;
}

void UNaItemStatics::GetEmptyItemEntry(FNaItemEntry & EmptyEntry) {
	EmptyEntry = EmptyItemEntry;
}

void UNaItemStatics::FindEntryFromIndex(const FNaItemContainer & Target, int Position, ENaItemContainerFindingResult & ReturnType, FNaItemEntry & Result) {
	FNaItemContainerFindingReturn Return = Target.Find(Position);
	ReturnType = Return.Result;
	Result = (ReturnType == ENaItemContainerFindingResult::ICFR_Filled) ? (*Return.EntryPtr.Get()) : EmptyItemEntry;
}

bool UNaItemStatics::IsValidItemEntry(const FNaItemEntry & ItemEntry) {
	return ItemEntry.IsValid();
}

bool UNaItemStatics::ResizeContainer(FNaItemContainer & Target, int NewSize, bool bForce) {
	return Target.Resize(NewSize, bForce);
}

bool UNaItemStatics::AddItemEntry(FNaItemContainer & Target, int Position, const FNaItemEntry & Entry, bool bForce) {
	return Target.AddEntry(Position, Entry, bForce);
}

int UNaItemStatics::AddOrStackItemEntry(FNaItemContainer & Target, int Position, const FNaItemEntry & Entry) {
	return Target.AddOrStack(Position, Entry);
}

void UNaItemStatics::RemoveItemEntry(FNaItemContainer & Target, int Position) {
	Target.RemoveEntry(Position);
}

bool UNaItemStatics::MoveItemEntry(FNaItemContainer & Target, int From, int To, bool bForce) {
	return Target.MoveEntry(From, To, bForce);
}

void UNaItemStatics::SwapItemEntry(FNaItemContainer & Target, int P1, int P2) {
	Target.SwapEntry(P1, P2);
}

