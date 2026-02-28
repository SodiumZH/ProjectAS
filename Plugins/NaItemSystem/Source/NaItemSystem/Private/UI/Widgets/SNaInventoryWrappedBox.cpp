#include "UI/Widgets/SNaInventoryWrappedBox.h"

#include "NatriumItemSystem.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "BPLibraries/NaItemDataStatics.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Components/NaItemInventoryComponent.h"
#include "UI/UMG/NaItemSlotList.h"
#include "Components/NaGameModeItemSystemComponent.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaInventoryWrappedBox::Construct(const FArguments& InArgs)
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
	
	StylePtr = InArgs._StylePtr.Get();
	bFillDisabledToCompleteRectangle = InArgs._bFillDisabledToCompleteRectangle.Get();
	RowLength = InArgs._RowLength.Get();
	RowCount = InArgs._RowCount.Get();

	/* Add panel */
	ChildSlot
		[
			SAssignNew(WrapBox, SWrapBox)
			.PreferredSize((StylePtr ? StylePtr->SlotSize.X : 0.f) * RowLength)
		];

	/* Reconstruct can do the rest */
	Reconstruct();

	BindEventsToUMG();
	
	bIsConstructed = true;
	return;
}
void SNaInventoryWrappedBox::Reconstruct() {

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

		UNaItemInventory* Inv = Container->GetInventory();
		const int32 ContainerSize = Inv->GetSize();

		/* Calculate actual length */
		// When row count is applied
		if (RowCount * RowLength > ContainerSize) {
			ActualLength = RowCount * RowLength;
		}
		// When additional disabled slots are not needed
		else if(!bFillDisabledToCompleteRectangle || ContainerSize % RowLength == 0) {
			ActualLength = ContainerSize;
		}
		// When disabled slots are needed and actual length needs to be automatically calculated (no rows with all disabled)
		else {
			ActualLength = (int(ContainerSize / RowLength) + 1) * RowLength;
		}
		// Re-add child slots
		int i = 0;
		Slots.Init(TSharedPtr<SNaItemSlot>(nullptr), ActualLength);
		// Add enabled slots first
		for (i = 0; i < ContainerSize; ++i) {
			WrapBox->AddSlot()[
				SAssignNew(Slots[i], SNaItemSlot)
					.StylePtr(StylePtr)
					.WorldContext(GMComponent)
					.Stack(Inv->GetSlot(i))
			];
		}
		// Then fill with disabled slots
		for (1; i < ActualLength; ++i) {
			WrapBox->AddSlot()[
				SAssignNew(Slots[i], SNaItemSlot)
					.StylePtr(StylePtr)
					.WorldContext(GMComponent)
					.bIsDisabled(true)
			];
		}
		for (i = 0; i < ActualLength; ++i) {
			Slots[i]->SetItemSlotList(this, i);
		}
	}


}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool SNaInventoryWrappedBox::IsUpdated(bool bDisplay) {
	
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
	if (Slots.Num() != Container->GetInventory()->GetSize()) {
		UE_LOG(LogNaItem, Display, TEXT("SNaItemSlotList: incorrect size."));
		return false;
	}

	// Check each slots
	for (i = 0; i < Slots.Num(); ++i) {
		if (Slots[i]->GetStack() != Container->GetInventory()->GetSlot(i)) {
			if(bDisplay)
				UE_LOG(LogNaItem, Display, TEXT("SNaItemSlotList: position %d is not updated."), i);
			Res = false;
		}
	}
	return Res;
}




void SNaInventoryWrappedBox::ResetSlot(int Position) {

	if (IsInvalid())
		return;

	if (!Container->GetInventory()->IsValidSlot(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("SNaItemSlotList::ResetSlot() failed: position out of range."));
		return;
	}


	Slots[Position]->ResetItemStack(Container->GetInventory()->GetSlot(Position));

}

void SNaInventoryWrappedBox::ResetAllSlots() {
	int i = 0;
	for (i = 0; i < Slots.Num(); ++i) {
		Slots[i]->ResetItemStack(Container->GetInventory()->GetSlot(i));
	}
}

void SNaInventoryWrappedBox::ResetContainer(UNaItemInventoryComponent* NewContainer) {
	Container = NewContainer;
	Reconstruct();
}

void SNaInventoryWrappedBox::SelectSlot(int Position) {
	if (!IsValid(Container)) return;
	check(Container->GetInventory()->IsValidSlot(Position));
	if (SelectedPosition >= 0) {
		Slots[SelectedPosition]->GetBoxSlot()->SetSelected(false);
	}
	SelectedPosition = Position;
	Slots[SelectedPosition]->GetBoxSlot()->SetSelected(true);
}

void SNaInventoryWrappedBox::UnselectAll() {
	if (SelectedPosition >= 0) {
		Slots[SelectedPosition]->GetBoxSlot()->SetSelected(false);
	}
	SelectedPosition = -1;
}

int SNaInventoryWrappedBox::GetSelectedPosition() {
	return SelectedPosition;
};

/* UNaItemSlotList event callers */

void SNaInventoryWrappedBox::SlotPointedToUMG(int Position) {
	UMGRef->OnSlotPointed.Broadcast(Position);
}
void SNaInventoryWrappedBox::SlotUnpointedToUMG(int Position) {
	UMGRef->OnSlotUnpointed.Broadcast(Position);
}
void SNaInventoryWrappedBox::SlotSelectedToUMG(int Position) {
	UMGRef->OnSlotSelected.Broadcast(Position);
}
void SNaInventoryWrappedBox::SlotUnselectedToUMG(int Position) {
	UMGRef->OnSlotUnselected.Broadcast(Position);
}
void SNaInventoryWrappedBox::SlotClickedToUMG(int Position) {
	UMGRef->OnSlotClicked.Broadcast(Position);
}
void SNaInventoryWrappedBox::SlotHoveredToUMG(int Position) {
	UMGRef->OnSlotHovered.Broadcast(Position);
}
void SNaInventoryWrappedBox::SlotUnhoveredToUMG(int Position) {
	UMGRef->OnSlotUnhovered.Broadcast(Position);
}

void SNaInventoryWrappedBox::SlotMouseButtonDownToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	UMGRef->OnSlotMouseButtonDown.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaInventoryWrappedBox::SlotMouseButtonUpToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	UMGRef->OnSlotMouseButtonUp.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaInventoryWrappedBox::SlotMouseMoveToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	UMGRef->OnSlotMouseMove.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaInventoryWrappedBox::SlotDoubleClickedToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	UMGRef->OnSlotDoubleClicked.Broadcast(Position, MyGeometry, MouseEvent);
}

void SNaInventoryWrappedBox::BindEventsToUMG() {
	if (IsValid(UMGRef)) {
		OnSlotPointed.AddSP(this, &SNaInventoryWrappedBox::SlotPointedToUMG);
		OnSlotUnpointed.AddSP(this, &SNaInventoryWrappedBox::SlotUnpointedToUMG);
		OnSlotSelected.AddSP(this, &SNaInventoryWrappedBox::SlotSelectedToUMG);
		OnSlotUnselected.AddSP(this, &SNaInventoryWrappedBox::SlotUnselectedToUMG);
		OnSlotClicked.AddSP(this, &SNaInventoryWrappedBox::SlotClickedToUMG);
		OnSlotHovered.AddSP(this, &SNaInventoryWrappedBox::SlotHoveredToUMG);
		OnSlotUnhovered.AddSP(this, &SNaInventoryWrappedBox::SlotUnhoveredToUMG);

		OnSlotMouseButtonDown.AddSP(this, &SNaInventoryWrappedBox::SlotMouseButtonDownToUMG);
		OnSlotMouseButtonUp.AddSP(this, &SNaInventoryWrappedBox::SlotMouseButtonUpToUMG);
		OnSlotMouseMove.AddSP(this, &SNaInventoryWrappedBox::SlotMouseMoveToUMG);
		OnSlotDoubleClicked.AddSP(this, &SNaInventoryWrappedBox::SlotDoubleClickedToUMG);
	}
}