// By Sodium


#include "UI/Widgets/SNaItemSlotList.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "NaUtilityMinimal.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Components/NaItemContainerComponent.h"
#include "UI/UMG/NaItemSlotList.h"

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
	UMGRef = InArgs._FromUMG;

	/* Add panel */
	ChildSlot
		[
			SAssignNew(WrapBox, SWrapBox)
			.PreferredSize(BoxSize.X * (RowLength + 0.5))
		];

	/* Reconstruct can do the rest */
	Reconstruct();

	BindEventsToUMG();
	
	bIsConstructed = true;
	return;
}
void SNaItemSlotList::Reconstruct() {

	// Clear all old slots
	Slots.Empty();
	WrapBox->ClearChildren();

	// Re-get game mode component from container
	GMComponent = IsValid(Container) ? UNaItemStatics::GetGameModeItemSystemComponent(Container) : nullptr;

	// When container is invalid, this widget is invalid
	if (!IsValid(Container)) {
		UE_LOG(LogNaItem, Display, TEXT("NaItemSlotList: invalid item container."));
		bIsInvalid = true;
	}
	// When game mode component is invalid, this widget is invalid
	else if (!IsValid(GMComponent)) {
		UE_LOG(LogNaItem, Warning, TEXT("NaItemSlotList: invalid game mode component."));
		bIsInvalid = true;
	}
	else bIsInvalid = false;


	// Start rebuild
	if (!bIsInvalid) {

		// Actual length needs to be re-calculated
		ActualLength = (!bFillDisabledToCompleteRectangle || Container->Container.GetSize() % RowLength == 0) ? Container->Container.GetSize() : (Container->Container.GetSize() - Container->Container.GetSize() % RowLength + RowLength);

		// Re-add child slots
		int i = 0;
		Slots.Init(TSharedPtr<SNaItemSlot>(nullptr), ActualLength);
		// Add enabled slots first
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
		// Then fill with disabled slots
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
		for (i = 0; i < ActualLength; ++i) {
			Slots[i]->SetItemSlotList(this, i);
		}
	}


}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SNaItemSlotList::IsUpdated(bool bDisplay) {
	
	bool Res = true;
	int i = 0;

	// When invalid, just skip
	if (!IsValid(Container) || (!IsValid(GMComponent))) {
		if(!IsInvalid())
			UE_LOG(LogNaItem, Warning, TEXT("SNaItemSlotList: invalid container or game mode component, when this list is not tagged invalid."));
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

	if (Container->Container.IsInSize(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("SNaItemSlotList::ResetSlot() failed: position out of range."));
		return;
	}


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
	if (!IsValid(Container)) return;
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

int SNaItemSlotList::GetSelectedPosition() {
	return SelectedPosition;
};

/* UNaItemSlotList event callers */

void SNaItemSlotList::SlotPointedToUMG(int Position) {
	UMGRef->OnSlotPointed.Broadcast(Position);
}
void SNaItemSlotList::SlotUnpointedToUMG(int Position) {
	UMGRef->OnSlotUnpointed.Broadcast(Position);
}
void SNaItemSlotList::SlotSelectedToUMG(int Position) {
	UMGRef->OnSlotSelected.Broadcast(Position);
}
void SNaItemSlotList::SlotUnselectedToUMG(int Position) {
	UMGRef->OnSlotUnselected.Broadcast(Position);
}
void SNaItemSlotList::SlotClickedToUMG(int Position) {
	UMGRef->OnSlotClicked.Broadcast(Position);
}
void SNaItemSlotList::SlotHoveredToUMG(int Position) {
	UMGRef->OnSlotHovered.Broadcast(Position);
}
void SNaItemSlotList::SlotUnhoveredToUMG(int Position) {
	UMGRef->OnSlotUnhovered.Broadcast(Position);
}

void SNaItemSlotList::SlotMouseButtonDownToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	UMGRef->OnSlotMouseButtonDown.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaItemSlotList::SlotMouseButtonUpToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	UMGRef->OnSlotMouseButtonUp.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaItemSlotList::SlotMouseMoveToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	UMGRef->OnSlotMouseMove.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaItemSlotList::SlotDoubleClickedToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	UMGRef->OnSlotDoubleClicked.Broadcast(Position, MyGeometry, MouseEvent);
}

void SNaItemSlotList::BindEventsToUMG() {
	if (IsValid(UMGRef)) {
		OnSlotPointed.AddSP(this, &SNaItemSlotList::SlotPointedToUMG);
		OnSlotUnpointed.AddSP(this, &SNaItemSlotList::SlotUnpointedToUMG);
		OnSlotSelected.AddSP(this, &SNaItemSlotList::SlotSelectedToUMG);
		OnSlotUnselected.AddSP(this, &SNaItemSlotList::SlotUnselectedToUMG);
		OnSlotClicked.AddSP(this, &SNaItemSlotList::SlotClickedToUMG);
		OnSlotHovered.AddSP(this, &SNaItemSlotList::SlotHoveredToUMG);
		OnSlotUnhovered.AddSP(this, &SNaItemSlotList::SlotUnhoveredToUMG);

		OnSlotMouseButtonDown.AddSP(this, &SNaItemSlotList::SlotMouseButtonDownToUMG);
		OnSlotMouseButtonUp.AddSP(this, &SNaItemSlotList::SlotMouseButtonUpToUMG);
		OnSlotMouseMove.AddSP(this, &SNaItemSlotList::SlotMouseMoveToUMG);
		OnSlotDoubleClicked.AddSP(this, &SNaItemSlotList::SlotDoubleClickedToUMG);
	}
}