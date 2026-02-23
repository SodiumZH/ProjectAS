
#include "NaItemContainer.h"
#include "NatriumUtility.h"
#include "NaItemEffect.h"
#include "NatriumItemSystem.h"
#include "GameFramework/Actor.h"


// Found item successfully
FNaItemContainerFindingReturn::FNaItemContainerFindingReturn(UNaItemStack* InStack) {
	Stack = InStack;
	Result = ENaItemContainerFindingResult::ICFR_Filled;
}

// Not found
FNaItemContainerFindingReturn::FNaItemContainerFindingReturn(NotFoundType Type) {
	Stack = nullptr;
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
	for (int i = 0; i < 64; ++i)
		Content.Emplace(nullptr);
}

FNaItemContainer::FNaItemContainer(int InSize) {
	Size = InSize;
	for (int i = 0; i < Size; ++i)
		Content.Emplace(nullptr);
}

FNaItemContainer::FNaItemContainer(const FNaItemContainer& CopyFrom) {
	Content.Empty();
	Size = CopyFrom.GetSize();
	for (int i = 0; i < Size; ++i) {
		if (CopyFrom.Content[i] && !CopyFrom.Content[i]->IsEmpty())
			Content.Emplace(UNaItemStack::CopyItemStack(nullptr, CopyFrom.Content[i].Get()));
		else
			Content.Emplace(nullptr);
	}
}

FNaItemContainer::FNaItemContainer(int InSize, const TMap<int, UNaItemStack*>& InitialContent) : FNaItemContainer(InSize) {
	for (const auto& Elem : InitialContent) {
		if (IsInSize(Elem.Key)) {
			Content[Elem.Key] = Elem.Value;
		}
	}
}

bool FNaItemContainer::Resize(int NewSize, bool bForce) {

	check(Content.Num() == Size);

	if (NewSize >= Size) {
		/* Expand: add empty entries */
		for (int i = Size; i < NewSize; ++i) {
			Content.Emplace(nullptr);
		}
		Size = NewSize;
		return true;
	}
	else {
		/* Shrink until arrive new size */
		if (bForce) {
			while (Content.Num() > NewSize)
				Content.RemoveAt(Content.Num() - 1);
			Size = NewSize;
			return true;
		}
		else {
			/* Check there isn't any valid items */
			for (int i = NewSize; i < Size; ++i) {
				if (Content[i] != nullptr && !Content[i]->IsEmpty())
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

	if (!Content[Index] || Content[Index]->IsEmpty())	// Entry is empty/null
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::Empty);
	if (!Content[Index]->IsValid())	// Entry is invalid
		return FNaItemContainerFindingReturn(FNaItemContainerFindingReturn::NotFoundType::InvVal);

	// Succeeded
	return FNaItemContainerFindingReturn(Content[Index].Get());
}



bool FNaItemContainer::FindByType(UNaItemType* Type, TArray<int>& Positions) {

	Positions.Empty();
	CheckSize();

	for (int i = 0; i < Size; ++i) {
		if (Content[i] && Content[i]->ItemType == Type)
			Positions.Add(i);
	}
	return Positions.Num() != 0;
}

void FNaItemContainer::ClearInvalid() {
	for (int i = 0; i < Size; ++i) {
		if (Content[i] && Content[i]->IsEmpty()) {
			UE_LOG(LogNaItem, Warning, TEXT("NaItemContainer ClearInvalid: Empty item stack detected. Automatically cleared."));
			Content[i] = nullptr;
		}
	}
}


bool FNaItemContainer::AddEntry(int Position, UNaItemStack* Stack, bool bForce) {

	CheckSize();

	// Check OOS	
	if (!IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("Add entry: out of size."));
		return false;
	}

	// Case if index is empty
	if (!Content[Position] || Content[Position]->IsEmpty()) {
		Content[Position] = Stack;
		return true;
	}

	// Case if occupied, but force add
	if (bForce) {
		UE_LOG(LogNaItem, Log, TEXT("Add entry: an entry is replaced due to bForce."));
		Content[Position] = Stack;
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

	Content[Position] = nullptr;
}

bool FNaItemContainer::MoveEntry(int From, int To, bool bForce) {

	CheckSize();

	// Check OOS	
	if (!IsInSize(From) || !IsInSize(To)) {
		UE_LOG(LogNaItem, Warning, TEXT("Move entry: out of size."));
		return false;
	}

	// Case when from is empty
	if (!Content[From] || Content[From]->IsEmpty()) {
		UE_LOG(LogNaItem, Log, TEXT("Move entry: trying to move empty entry. Use RemoveEntry() to empty a position."));
		return false;
	}

	// Case when to is occupied
	if (Content[To] && !Content[To]->IsEmpty()) {
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

	TObjectPtr<UNaItemStack> Temp = Content[P2];
	Content[P2] = Content[P1];
	Content[P1] = Temp;
}

/*- Data-dependent Operations Below -*/

int FNaItemContainer::AddOrStack(int Position, UNaItemStack* Stack) {

	CheckSize();

	if (!Stack || Stack->IsEmpty())
		return 0;

	// OOS case
	if (!IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("Add or Stack: index out of size."));
		return Stack->Count;
	}

	// Empty, add
	if (!Content[Position] || Content[Position]->IsEmpty()) {
		Content[Position] = Stack;
		return 0;
	}

	// Containing the same item type, stack
	if (Content[Position]->CanStackWith(Stack, false)) {
		return Stack->Count - Content[Position]->MergeFrom(Stack, true);
	}

	// Mismatch, do nothing and return the input
	return Stack->Count;
}

int FNaItemContainer::GiveItem(UNaItemStack* Stack) {

	ClearInvalid();

	if (!Stack || Stack->IsEmpty())
		return 0;

	int AmountLeft = Stack->Count;
	UNaItemType* Type = Stack->ItemType;
	int MaxStacking = Stack->GetMaxStackSize();

	// Try stacking onto existing stacks
	for (int i = 0; i < Size && AmountLeft > 0; ++i) {
		if (!Content[i] || Content[i]->IsEmpty())
			continue;
		if (Content[i]->ItemType != Type)
			continue;
		// Fully occupied, skip
		if (Content[i]->IsFullStack())
			continue;

		int CanAdd = Content[i]->GetRemainingCapacity();
		int ToAdd = FMath::Min(CanAdd, AmountLeft);
		Content[i]->Grow(ToAdd);
		AmountLeft -= ToAdd;
	}

	// Try adding to empty slots
	for (int i = 0; i < Size && AmountLeft > 0; ++i) {
		if (Content[i] && !Content[i]->IsEmpty())
			continue;

		int ToAdd = FMath::Min(AmountLeft, MaxStacking);
		UNaItemStack* NewStack = UNaItemStack::CreateItemStack(nullptr, Type, ToAdd);
		Content[i] = NewStack;
		AmountLeft -= ToAdd;
	}

	return AmountLeft;
}

bool FNaItemContainer::CanGiveItemComplete(const UNaItemStack* Stack) const {
	FNaItemContainer ContainerCopy(*this);
	UNaItemStack* StackCopy = UNaItemStack::CopyItemStack(nullptr, Stack);
	int res = ContainerCopy.GiveItem(StackCopy);
	return res == 0;
}

bool FNaItemContainer::GiveItemComplete(UNaItemStack* Stack) {
	if (CanGiveItemComplete(Stack)) {
		GiveItem(Stack);
		return true;
	}
	else return false;
}

bool FNaItemContainer::CanGiveItemMultiComplete(const TArray<UNaItemStack*>& Stacks) {
	FNaItemContainer ContainerCopy(*this);
	for (UNaItemStack* Stack : Stacks) {
		if (!Stack || Stack->IsEmpty())
			continue;
		UNaItemStack* StackCopy = UNaItemStack::CopyItemStack(nullptr, Stack);
		int temp = ContainerCopy.GiveItem(StackCopy);
		if (temp > 0) {
			return false;
		}
	}
	return true;
}

ENaItemContainerUsageResult FNaItemContainer::PreUsageProcess(UObject* WorldContext, int Position, AActor* Source, AActor* Target) {

	if (!IsInSize(Position) || !Content[Position] || Content[Position]->IsEmpty())
		return ENaItemContainerUsageResult::ICUR_Empty;

	return ENaItemContainerUsageResult::ICUR_Succeeded;
}

ENaItemContainerUsageResult FNaItemContainer::ExecuteUseItem(UObject* WorldContext, int Position, class AActor* Source, AActor* Target, ENaItemContainerUsageResult PreUsageResult) {
	if (PreUsageResult == ENaItemContainerUsageResult::ICUR_Succeeded) {
		bool bUsed = Content[Position]->Use(Source, Target);
		return bUsed ? ENaItemContainerUsageResult::ICUR_Succeeded : ENaItemContainerUsageResult::ICUR_NotUsable;
	}
	else return PreUsageResult;
}

ENaItemContainerUsageResult FNaItemContainer::PostUsageProcess(UObject* WorldContext, int Position, class AActor* Source, AActor* Target, ENaItemContainerUsageResult UsageResult) {
	if (UsageResult == ENaItemContainerUsageResult::ICUR_Succeeded) {
		// Try to consume one item
		if (Content[Position] && !Content[Position]->IsEmpty()) {
			Content[Position]->Consume(Source);
		}
		// If stack is empty after consumption, clear it
		if (!Content[Position] || Content[Position]->IsEmpty())
			Content[Position] = nullptr;
	}
	return UsageResult;
}

ENaItemContainerUsageResult FNaItemContainer::UseItem(UObject* WorldContext, int Position, AActor* Source, AActor* Target) {
	ENaItemContainerUsageResult PreRes = PreUsageProcess(WorldContext, Position, Source, Target);
	ENaItemContainerUsageResult UseRes = ExecuteUseItem(WorldContext, Position, Source, Target, PreRes);
	ENaItemContainerUsageResult PostRes = PostUsageProcess(WorldContext, Position, Source, Target, UseRes);
	return PostRes;
}



