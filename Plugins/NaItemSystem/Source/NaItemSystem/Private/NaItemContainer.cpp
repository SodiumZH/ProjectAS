
#include "NaItemContainer.h"
#include "NaUtility.h"
#include "NaItemEntry.h"
#include "NaItemEffect.h"
#include "Data/NaItemType.h"
#include "Data/NaItemEffectData.h"
#include "Engine/DataTable.h"
#include "Components/NaGameModeItemSystemComponent.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "GameFramework/Actor.h"


// Found item successfully
FNaItemContainerFindingReturn::FNaItemContainerFindingReturn(TSharedPtr<FNaItemEntry> InEntryPtr) {
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


/**** FNaItemContainer ****/

FNaItemContainer::FNaItemContainer() {
	Size = 64;
	int i = 0;
	for (i = 0; i < 64; ++i)
		Content.Emplace(TSharedPtr<FNaItemEntry>(nullptr));
}

FNaItemContainer::FNaItemContainer(int InSize) {
	Size = InSize;
	int i = 0;
	for (i = 0; i < Size; ++i)
		Content.Emplace(TSharedPtr<FNaItemEntry>(nullptr));
}

FNaItemContainer::FNaItemContainer(const FNaItemContainer & CopyFrom) {
	Content.Empty();
	Size = CopyFrom.GetSize();
	int i = 0;
	for (i = 0; i < Size; ++i) {
		if (CopyFrom.Find(i).Result == ENaItemContainerFindingResult::ICFR_Filled) 
			Content.Emplace(TSharedPtr<FNaItemEntry>(new FNaItemEntry(*(CopyFrom.Find(i).EntryPtr.Get()))));
		else Content.Emplace(TSharedPtr<FNaItemEntry>(nullptr));
	}
}

FNaItemContainer::FNaItemContainer(int InSize, const TMap<int, FNaItemEntry> & InitialContent ) : FNaItemContainer(InSize){
	for (auto & Elem : InitialContent) {
		if (IsInSize(Elem.Key)) {
			Content[Elem.Key] = TSharedPtr<FNaItemEntry>(new FNaItemEntry(Elem.Value));
		}
	}
}

bool FNaItemContainer::Resize(int NewSize, bool bForce) {
	
	check(Content.Num() == Size);

	if (NewSize >= Size) {
		/* Expand: add empty entries */
		int i = 0;
		for (i = Size; i < NewSize; ++i) {
			Content.Emplace(TSharedPtr<FNaItemEntry>(nullptr));
		}
		Size = NewSize;
		return true;
	}
	else {
		/* Shrink until arrive new size */
		if (bForce) {
			while (Content.Num() > NewSize)
				Content.RemoveAt(Content.Num() - 1);
			return true;
		}
		else {
			int i = 0;
			/* Check there isn't any valid items */
			for (i = NewSize; i < Size; ++i) {
				if (Content[i].IsValid())
					return false;
			}
			while (Content.Num() > NewSize)
				Content.RemoveAt(Content.Num() - 1);
		}
		Size = NewSize;
		return true;
	}
}

FNaItemContainerFindingReturn FNaItemContainer::Find(int Index) const {
	
	/* Out of size check */
	if (Index < 0 || Index > ITEM_CONTAINER_MAX_SIZE)
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::InvID);
	
	if (Index >= Size)
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::OOS);
	
	if (!Content[Index].IsValid())	// Entry is empty/null
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::Empty);
	if (!Content[Index]->IsValid())	// Entry is invalid
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::InvVal);

	// Succeeded
	return FNaItemContainerFindingReturn(Content[Index]);
}



bool FNaItemContainer::FindItem(int TypeIndex, TArray<int>& Positions, bool bIncludeUniquified) {
	
	Positions.Empty();
	int i = 0;
	CheckSize();

	for (i = 0; i < Size; ++i) {
		if (!Content[i].IsValid())
			continue;
		else if (Content[i]->TypeDescriptor.ItemTypeID == TypeIndex)
			if (!Content[i]->TypeDescriptor.bIsUniquified || bIncludeUniquified)
				Positions.Add(i);
	}
	return Positions.Num() != 0;
}

bool FNaItemContainer::FindItemExplicit(const FNaItemDescriptor & Type, TArray<int>& Positions) {
	
	Positions.Empty();
	int i = 0;
	CheckSize();

	for (i = 0; i < Size; ++i) {
		if (!Content[i].IsValid())
			continue;
		else if (Content[i]->TypeDescriptor == Type)
			Positions.Add(i);
	}
	return Positions.Num() != 0;
}


bool FNaItemContainer::AddEntry(int Position, const FNaItemEntry & Entry, bool bForce) {

	CheckSize();

	// Check OOS	
	if (!IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("Add entry: out of size."));
		return false;
	}

	// Case if index is empty
	if (!Content[Position].IsValid()){
		Content[Position] = TSharedPtr<FNaItemEntry>(new FNaItemEntry(Entry));	// Redirect the ptr to a created entry object
		return true;
	}

	// Case if occupied, but force add
	if (bForce) {
		UE_LOG(LogNaItem, Log, TEXT("Add entry: an entry is replaced due to bForce."));
		Content[Position] = TSharedPtr<FNaItemEntry>(new FNaItemEntry(Entry));
		return true;
	}

	// Occupied, don't add
	return false;
}

void FNaItemContainer::RemoveEntry(int Position) {

	CheckSize();

	// Check OOS	
	if (!IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("Remove entry: out of size."));
		return;
	}

	// Already empty
	if (!Content[Position].IsValid())
		return;

	// Occupied
	else Content[Position] = TSharedPtr<FNaItemEntry>(nullptr);

}

bool FNaItemContainer::MoveEntry(int From, int To, bool bForce) {

	CheckSize();

	// Check OOS	
	if (!IsInSize(From) || !IsInSize(To)) {
		UE_LOG(LogNaItem, Warning, TEXT("Move entry: out of size."));
		return false;
	}
	
	// Case when from is empty
	if (!Content[From].IsValid()) {
		UE_LOG(LogNaItem, Log, TEXT("Move entry: trying to move empty entry. Use RemoveEntry() to empty a position."));
		return false;
	}

	// Case when to is occupied
	if (Content[To].IsValid()) {
		if (bForce) {
			Content[To] = Content[From];
			Content[From] = nullptr;
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
		Content[To] = Content[From];
		Content[From] = nullptr;
		return true;
	}
}

void FNaItemContainer::SwapEntry(int P1, int P2) {

	CheckSize();

	// Check OOS
	if (!IsInSize(P1) || !IsInSize(P2)) {
		UE_LOG(LogNaItem, Warning, TEXT("Swap entry: out of size."));
		return;
	}



	// P1 is empty
	if (!Content[P1].IsValid()) {
		if (!Content[P2].IsValid())
			// Two empties, do nothing
			return;
		else {
			Content[P1] = Content[P2];
			Content[P2] = nullptr;
		}
	}
	// P1 isn't empty, P2 is empty
	else if (!Content[P2].IsValid()) {
		Content[P2] = Content[P1];
		Content[P1] = nullptr;
	}
	// both occupied
	else {
		TSharedPtr<FNaItemEntry> Temp = Content[P2];
		Content[P2] = Content[P1];
		Content[P1] = Temp;
	}

}

/*- Data-dependent Operations Below -*/

bool FNaItemContainer::CheckStacking(UObject* WorldContext) {
	int i = 0;
	for (i = 0; i < Size; ++i) {
		if (!Content[i].IsValid())
			continue;
		if (Content[i]->Amount <= 0 ||
			Content[i]->Amount > UNaItemDataStatics::GetItemTypeFromID(WorldContext, Content[i]->TypeDescriptor.ItemTypeID).GetTypeData().MaxStackingAmount)
			return false;
	}
	return true;
}

int FNaItemContainer::AddOrStack(UObject* WorldContext, int Position, const FNaItemEntry & Entry) {

	CheckSize();

	// OOS case
	if (!IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("Add or Stack: index out of size."));
		return Entry.Amount;
	}

	// Empty, add
	if (!Content[Position].IsValid()) {
		AddEntry(Position, Entry);
		check(CheckStacking(WorldContext));
		return 0;
	}

	// Containing the same item, stack
	if (Content[Position]->TypeDescriptor == Entry.TypeDescriptor) {
		FNaItemType Type = UNaItemDataStatics::GetItemTypeFromID(WorldContext, Content[Position]->TypeDescriptor.ItemTypeID);
		
		// If type is invalid, report error and abort. This case should not happen. 
		if (!Type.IsValid()) {
			UE_LOG(LogNaItem, Error, TEXT("NaItemContainer Error: type of item entry on position &d is invalid."), Position);
			return Entry.Amount;
		}
		// When space is enough for all items input
		if (Type.GetTypeData().MaxStackingAmount - Content[Position]->Amount >= Entry.Amount) {
			Content[Position]->Amount += Entry.Amount;
			check(CheckStacking(WorldContext));
			return 0;
		}
		// Not enough, stack as possible and return the rest
		else {
			int RestAmount = Content[Position]->Amount + Entry.Amount - Type.GetTypeData().MaxStackingAmount;
			Content[Position]->Amount = Type.GetTypeData().MaxStackingAmount;
			check(CheckStacking(WorldContext));
			return RestAmount;
		}
	}
	// Mismatch, do nothing and return the input
	check(CheckStacking(WorldContext));
	return Entry.Amount;
}

FNaItemUsageReturn FNaItemContainer::UseItem(UObject* WorldContext, int Position, AActor* Source, AActor* Target) {
	check(CheckStacking(WorldContext));
	if (!Content[Position].IsValid()) {
		UE_LOG(LogNaItem, Log, TEXT("NaItemContainer using item: using item from an empty slot."));
		return FNaItemUsageReturn::Null();
	}
	UClass* EffectClass = UNaItemDataStatics::GetItemEffectDataFromID(WorldContext, Content[Position]->TypeDescriptor.ItemTypeID).EffectClass.Get();
	FNaItemUsageReturn res = Cast<UNaItemEffect>(EffectClass->GetDefaultObject())->UseItem(Content[Position]->TypeDescriptor.ItemTypeID, Source, Target, Position);
	checkf(CheckStacking(WorldContext), TEXT("NaItemContainer using item error: Item stacking amount error after usage. Position: %d, Item ID: %d"), Position, Content[Position]->TypeDescriptor.ItemTypeID);
	return res;
}