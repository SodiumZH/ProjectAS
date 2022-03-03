// By Sodium


#include "UI/Widgets/SNaItemSlotList.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "NaUtilityMinimal.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Components/NaItemContainerComponent.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaItemSlotList::Construct(const FArguments& InArgs)
{

	Container = InArgs._Container.Get();

	// When container is invalid, this widget is invalid
	if (!IsValid(Container)) {
		UE_LOG(LogNaItem, Display, TEXT("NaItemSlotList: invalid item container."));
		bIsInvalid = true;
	}

	/* Get context and container information */

	GMComponent = bIsInvalid ? nullptr : UNaItemStatics::GetGameModeItemSystemComponent(InArgs._Container.Get());

	// When game mode component is invalid, this widget is invalid
	if (!IsValid(GMComponent)) {
		if (!bIsInvalid)
			UE_LOG(LogNaItem, Warning, TEXT("NaItemSlotList: invalid game mode component."));
		bIsInvalid = true;
	}




	/* Copy inputs */
	BoxSize = InArgs._BoxSize.Get();
	bHideAmountWhenOne = InArgs._bHideAmountWhenOne.Get();
	Font = InArgs._Font.Get();
	bFillDisabledToCompleteRectangle = InArgs._bFillDisabledToCompleteRectangle.Get();
	RowLength = InArgs._RowLength.Get();


	/* Add panel */
	ChildSlot
		[
			SAssignNew(WrapBox, SWrapBox)
			.PreferredSize(BoxSize.X * (RowLength + 0.5))
		];

	/* Reconstruct can do the rest */
	Reconstruct();
		
	return;
}
void SNaItemSlotList::Reconstruct() {

	// Clear all old slots
	Slots.Empty();
	WrapBox->ClearChildren();

	// When container is invalid, this widget is invalid
	if (!IsValid(Container)) {
		UE_LOG(LogNaItem, Display, TEXT("NaItemSlotList: invalid item container."));
		bIsInvalid = true;
	}

	// Re-get game mode component from container *
	GMComponent = IsValid(Container) ? UNaItemStatics::GetGameModeItemSystemComponent(Container) : nullptr;

	// When game mode component is invalid, this widget is invalid
	if (!IsValid(Container) || !IsValid(GMComponent)) {
		if (!bIsInvalid)
			UE_LOG(LogNaItem, Warning, TEXT("NaItemSlotList: invalid game mode component."));
		bIsInvalid = true;
	}
	else bIsInvalid = false;


	// Start rebuild
	if (!bIsInvalid) {

		// Actual length needs to be re-calculated
		ActualLength = (!bFillDisabledToCompleteRectangle || Container->Container.GetSize() % RowLength == 0) ? Container->Container.GetSize() : (Container->Container.GetSize() / RowLength * RowLength) + 1;

		// Re-add child slots
		int i = 0;
		Slots.Init(TSharedPtr<SNaItemSlot>(nullptr), ActualLength);
		for (i = 0; i < Container->Container.GetSize(); ++i) {
			WrapBox->AddSlot()[
				SAssignNew(Slots[i], SNaItemSlot)
					.WorldContext(GMComponent)
					.EntryPtr(Container->Container.Find(i).EntryPtr)
					.Size(BoxSize)
					.bHideAmountWhenOne(bHideAmountWhenOne)
					.Font(Font)
			];
		}
		for (1; i < ActualLength; ++i) {
			WrapBox->AddSlot()[
				SAssignNew(Slots[i], SNaItemSlot)
					.WorldContext(GMComponent)
					.Size(BoxSize)
					.bHideAmountWhenOne(bHideAmountWhenOne)
					.Font(Font)
					.bIsDisabled(true)
			];
		}
	}
	if (bIsConstructed) {
		PostConstructionInit();
	}
}

void SNaItemSlotList::PostConstructionInit() {

	// Setup item slot list for each slots 
	// to allow them to apply changes from list
	int i;
	for (i = 0; i < ActualLength; ++i) {
		Slots[i]->SetItemSlotList(TSharedPtr<SNaItemSlotList>(this), i);
	}

	OnPostConstructionInit.ExecuteIfBound();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SNaItemSlotList::IsUpdated(bool bDisplay) {
	
	bool Res = true;
	int i = 0;

	// When invalid, just skip
	if (!IsValid(Container) || (!IsValid(GMComponent))) {
		if(!IsInvalid())
			UE_LOG(LogNaItem, Warning, TEXT("SNaItemSlotList: invalid container or game mode component, while not invalidated."));
		return true;
	}
	// Case when references are correct but widget is invalid
	else if (IsInvalid()) {
		UE_LOG(LogNaItem, Display, TEXT("SNaItemSlotList: invalid widget when container and game mode component are correct."));
		return false;
	}

	// Check size
	if (Slots.Num() != Container->Container.GetSize()) {
		UE_LOG(LogNaItem, Display, TEXT("SNaItemSlotList: incorrect size."));
		return false;
	}

	// Check each slots
	for (i = 0; i < Slots.Num(); ++i) {
		if (Slots[i]->GetEntryPtr() != Container->Container.Find(i).EntryPtr) {
			if(bDisplay)
				UE_LOG(LogNaItem, Display, TEXT("SNaItemSlotList: position %d is not updated."), i);
			Res = false;
		}
	}
	return Res;
}




void SNaItemSlotList::ResetSlot(int Position) {

	if (IsInvalid())
		return;

#if NAPACK_DO_VERBOSE_CHECK
	if (Container->Container.IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("SNaItemSlotList::ResetSlot() failed: position out of range."));
		return;
	}
#else
#if NAPACK_DO_COMMON_CHECK
	check(Container->Container.IsInSize(Position));
#endif
#endif

	Slots[Position]->ResetItemEntry(Container->Container.Find(Position).EntryPtr);

}

void SNaItemSlotList::ResetAllSlots() {
	int i = 0;
	for (i = 0; i < Slots.Num(); ++i) {
		Slots[i]->ResetItemEntry(Container->Container.Find(i).EntryPtr);
	}
}

void SNaItemSlotList::ResetContainer(UNaItemContainerComponent* NewContainer) {
	Container = NewContainer;
	Reconstruct();
}

void SNaItemSlotList::SelectSlot(int Position) {
	check(Container->Container.IsInSize(Position));
	if (SelectedPosition >= 0) {
		Slots[SelectedPosition]->GetBoxSlot()->SetSelected(false);
	}
	SelectedPosition = Position;
	Slots[SelectedPosition]->GetBoxSlot()->SetSelected(true);
}

void SNaItemSlotList::UnselectAll() {
	if (SelectedPosition >= 0) {
		Slots[SelectedPosition]->GetBoxSlot()->SetSelected(false);
	}
	SelectedPosition = -1;
}