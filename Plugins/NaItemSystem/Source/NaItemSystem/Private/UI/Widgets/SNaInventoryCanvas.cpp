// By Sodium

#include "UI/Widgets/SNaInventoryCanvas.h"

#include "NatriumItemSystem.h"
#include "SlateOptMacros.h"
#include "BPLibraries/NaItemStatics.h"
#include "Widgets/SCanvas.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "Components/NaItemInventoryComponent.h"
#include "UI/UMG/NaInventoryCanvas.h"
#include "Components/NaGameModeItemSystemComponent.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SNaInventoryCanvas::Construct(const FArguments& InArgs)
{
	Container = InArgs._Container.Get();

	// When container is invalid, this widget is invalid
	if (!IsValid(Container)) {
		UE_LOG(LogNaItem, Display, TEXT("SNaInventoryCanvas: invalid item container."));
		bIsInvalid = true;
	}

	/* Get context */
	GMComponent = bIsInvalid ? nullptr : UNaItemStatics::GetGameModeItemSystemComponent(InArgs._Container.Get());

	if (!IsValid(GMComponent)) {
		if (!bIsInvalid)
			UE_LOG(LogNaItem, Warning, TEXT("SNaInventoryCanvas: invalid game mode component."));
		bIsInvalid = true;
	}

	/* Copy inputs */
	StylePtr = InArgs._StylePtr.Get();
	SlotAmount = InArgs._SlotAmount.Get();
	UMGRef = InArgs._FromUMG;

	/* Create canvas panel */
	ChildSlot
		[
			SAssignNew(Canvas, SCanvas)
		];

	/* Reconstruct can do the rest */
	Reconstruct();

	BindEventsToUMG();

	bIsConstructed = true;
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FVector2D SNaInventoryCanvas::GetSlotPosition(int32 Index) const
{
	return FVector2D::ZeroVector;
}

void SNaInventoryCanvas::Reconstruct()
{
	// Clear all old slots
	Slots.Empty();
	Canvas->ClearChildren();

	// Re-get game mode component from container
	GMComponent = IsValid(Container) ? UNaItemStatics::GetGameModeItemSystemComponent(Container) : nullptr;

	// When container is invalid, this widget is invalid
	if (!IsValid(Container)) {
		UE_LOG(LogNaItem, Display, TEXT("SNaInventoryCanvas: invalid item container."));
		bIsInvalid = true;
	}
	else if (!IsValid(GMComponent)) {
		UE_LOG(LogNaItem, Warning, TEXT("SNaInventoryCanvas: invalid game mode component."));
		bIsInvalid = true;
	}
	else bIsInvalid = false;

	if (!bIsInvalid) {

		UNaItemInventory* Inv = Container->GetInventory();
		const int32 InvSize = Inv->GetSize();

		// Determine actual slot count
		const int32 ActualSlotAmount = (SlotAmount > 0) ? SlotAmount : InvSize;

		const FVector2D SlotSize = StylePtr ? StylePtr->SlotSize : FVector2D(64.f, 64.f);

		Slots.Init(TSharedPtr<SNaItemSlot>(nullptr), ActualSlotAmount);

		for (int32 i = 0; i < ActualSlotAmount; ++i) {

			const FVector2D Pos = GetSlotPosition(i);
			const bool bDisabled = (i >= InvSize);

			if (bDisabled) {
				Canvas->AddSlot()
					.Position(Pos)
					.Size(SlotSize)
					[
						SAssignNew(Slots[i], SNaItemSlot)
							.StylePtr(StylePtr)
							.WorldContext(GMComponent)
							.bIsDisabled(true)
					];
			}
			else {
				Canvas->AddSlot()
					.Position(Pos)
					.Size(SlotSize)
					[
						SAssignNew(Slots[i], SNaItemSlot)
							.StylePtr(StylePtr)
							.WorldContext(GMComponent)
							.Stack(Inv->GetSlot(i))
					];
			}

			BindSlotEvents(i);
		}
	}
}

void SNaInventoryCanvas::BindSlotEvents(int32 Position)
{
	TSharedPtr<SNaItemSlot> Slot = Slots[Position];
	if (!Slot.IsValid()) return;

	TSharedPtr<SNaBoxSlot> BoxSlotWidget = Slot->GetBoxSlot();
	if (!BoxSlotWidget.IsValid()) return;

	// Only bind events for valid inventory slots
	if (!IsValid(Container) || !Container->GetInventory()->IsValidSlot(Position)) {
		return;
	}

	// Bind pointing and selection events with Position payload
	// Use BindRaw/CreateRaw instead of BindSP/CreateSP to avoid cyclic TSharedPtr references
	// (Canvas → Slots → SNaItemSlot → BoxSlot → delegates → Canvas)
	BoxSlotWidget->OnPointed.BindRaw(this, &SNaInventoryCanvas::HandleSlotPointed, Position);
	BoxSlotWidget->OnUnpointed.BindRaw(this, &SNaInventoryCanvas::HandleSlotUnpointed, Position);
	BoxSlotWidget->OnSelected.BindRaw(this, &SNaInventoryCanvas::HandleSlotSelected, Position);
	BoxSlotWidget->OnUnselected.BindRaw(this, &SNaInventoryCanvas::HandleSlotUnselected, Position);

	// Bind button events
	BoxSlotWidget->GetButton()->SetOnClicked(FOnClicked::CreateRaw(this, &SNaInventoryCanvas::HandleSlotClicked, Position));
	BoxSlotWidget->GetButton()->SetOnHovered(FSimpleDelegate::CreateRaw(this, &SNaInventoryCanvas::HandleSlotHovered, Position));
	BoxSlotWidget->GetButton()->SetOnUnhovered(FSimpleDelegate::CreateRaw(this, &SNaInventoryCanvas::HandleSlotUnhovered, Position));

	// Bind mouse events
	BoxSlotWidget->GetButton()->SetOnMouseButtonDown(FPointerEventHandler::CreateRaw(this, &SNaInventoryCanvas::HandleSlotMouseButtonDown, Position));
	BoxSlotWidget->GetButton()->SetOnMouseButtonUp(FPointerEventHandler::CreateRaw(this, &SNaInventoryCanvas::HandleSlotMouseButtonUp, Position));
	BoxSlotWidget->GetButton()->SetOnMouseMove(FPointerEventHandler::CreateRaw(this, &SNaInventoryCanvas::HandleSlotMouseMove, Position));
	BoxSlotWidget->GetButton()->SetOnMouseDoubleClick(FPointerEventHandler::CreateRaw(this, &SNaInventoryCanvas::HandleSlotDoubleClicked, Position));
}

/* Slot event handlers */

void SNaInventoryCanvas::HandleSlotPointed(int32 Position) {
	OnSlotPointed.Broadcast(Position);
}
void SNaInventoryCanvas::HandleSlotUnpointed(int32 Position) {
	OnSlotUnpointed.Broadcast(Position);
}
void SNaInventoryCanvas::HandleSlotSelected(int32 Position) {
	OnSlotSelected.Broadcast(Position);
}
void SNaInventoryCanvas::HandleSlotUnselected(int32 Position) {
	OnSlotUnselected.Broadcast(Position);
}
FReply SNaInventoryCanvas::HandleSlotClicked(int32 Position) {
	OnSlotClicked.Broadcast(Position);
	return FReply::Handled();
}
void SNaInventoryCanvas::HandleSlotHovered(int32 Position) {
	// Replicate the SetPointed(true) behavior from SNaItemSlot::SlotHoveredToList
	if (Slots.IsValidIndex(Position) && Slots[Position].IsValid()) {
		Slots[Position]->GetBoxSlot()->SetPointed(true);
	}
	OnSlotHovered.Broadcast(Position);
}
void SNaInventoryCanvas::HandleSlotUnhovered(int32 Position) {
	// Replicate the SetPointed(false) behavior from SNaItemSlot::SlotUnhoveredToList
	if (Slots.IsValidIndex(Position) && Slots[Position].IsValid()) {
		Slots[Position]->GetBoxSlot()->SetPointed(false);
	}
	OnSlotUnhovered.Broadcast(Position);
}

FReply SNaInventoryCanvas::HandleSlotMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, int32 Position) {
	OnSlotMouseButtonDown.Broadcast(Position, MyGeometry, MouseEvent);
	return FReply::Handled();
}
FReply SNaInventoryCanvas::HandleSlotMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, int32 Position) {
	OnSlotMouseButtonUp.Broadcast(Position, MyGeometry, MouseEvent);
	return FReply::Handled();
}
FReply SNaInventoryCanvas::HandleSlotMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, int32 Position) {
	OnSlotMouseMove.Broadcast(Position, MyGeometry, MouseEvent);
	return FReply::Handled();
}
FReply SNaInventoryCanvas::HandleSlotDoubleClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, int32 Position) {
	OnSlotDoubleClicked.Broadcast(Position, MyGeometry, MouseEvent);
	return FReply::Handled();
}

void SNaInventoryCanvas::ResetSlot(int Position)
{
	if (IsInvalid())
		return;

	if (!Container->GetInventory()->IsValidSlot(Position)) {
		UE_LOG(LogNaItem, Warning, TEXT("SNaInventoryCanvas::ResetSlot() failed: position out of range."));
		return;
	}

	Slots[Position]->ResetItemStack(Container->GetInventory()->GetSlot(Position));
}

void SNaInventoryCanvas::ResetAllSlots()
{
	if (IsInvalid())
		return;

	UNaItemInventory* Inv = Container->GetInventory();
	for (int32 i = 0; i < Slots.Num() && i < Inv->GetSize(); ++i) {
		Slots[i]->ResetItemStack(Inv->GetSlot(i));
	}
}

void SNaInventoryCanvas::ResetContainer(UNaItemInventoryComponent* NewContainer)
{
	Container = NewContainer;
	Reconstruct();
}

void SNaInventoryCanvas::SelectSlot(int Position)
{
	if (!IsValid(Container)) return;
	check(Container->GetInventory()->IsValidSlot(Position));
	if (SelectedPosition >= 0) {
		Slots[SelectedPosition]->GetBoxSlot()->SetSelected(false);
	}
	SelectedPosition = Position;
	Slots[SelectedPosition]->GetBoxSlot()->SetSelected(true);
}

void SNaInventoryCanvas::UnselectAll()
{
	if (SelectedPosition >= 0) {
		Slots[SelectedPosition]->GetBoxSlot()->SetSelected(false);
	}
	SelectedPosition = -1;
}

int SNaInventoryCanvas::GetSelectedPosition()
{
	return SelectedPosition;
}

/* UMG event callers */

void SNaInventoryCanvas::SlotPointedToUMG(int Position) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotPointed.Broadcast(Position);
}
void SNaInventoryCanvas::SlotUnpointedToUMG(int Position) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotUnpointed.Broadcast(Position);
}
void SNaInventoryCanvas::SlotSelectedToUMG(int Position) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotSelected.Broadcast(Position);
}
void SNaInventoryCanvas::SlotUnselectedToUMG(int Position) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotUnselected.Broadcast(Position);
}
void SNaInventoryCanvas::SlotClickedToUMG(int Position) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotClicked.Broadcast(Position);
}
void SNaInventoryCanvas::SlotHoveredToUMG(int Position) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotHovered.Broadcast(Position);
}
void SNaInventoryCanvas::SlotUnhoveredToUMG(int Position) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotUnhovered.Broadcast(Position);
}

void SNaInventoryCanvas::SlotMouseButtonDownToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotMouseButtonDown.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaInventoryCanvas::SlotMouseButtonUpToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotMouseButtonUp.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaInventoryCanvas::SlotMouseMoveToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotMouseMove.Broadcast(Position, MyGeometry, MouseEvent);
}
void SNaInventoryCanvas::SlotDoubleClickedToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) {
	if (UMGRef.IsValid())
		UMGRef->OnSlotDoubleClicked.Broadcast(Position, MyGeometry, MouseEvent);
}

void SNaInventoryCanvas::BindEventsToUMG()
{
	if (UMGRef.IsValid()) {
		OnSlotPointed.AddRaw(this, &SNaInventoryCanvas::SlotPointedToUMG);
		OnSlotUnpointed.AddRaw(this, &SNaInventoryCanvas::SlotUnpointedToUMG);
		OnSlotSelected.AddRaw(this, &SNaInventoryCanvas::SlotSelectedToUMG);
		OnSlotUnselected.AddRaw(this, &SNaInventoryCanvas::SlotUnselectedToUMG);
		OnSlotClicked.AddRaw(this, &SNaInventoryCanvas::SlotClickedToUMG);
		OnSlotHovered.AddRaw(this, &SNaInventoryCanvas::SlotHoveredToUMG);
		OnSlotUnhovered.AddRaw(this, &SNaInventoryCanvas::SlotUnhoveredToUMG);

		OnSlotMouseButtonDown.AddRaw(this, &SNaInventoryCanvas::SlotMouseButtonDownToUMG);
		OnSlotMouseButtonUp.AddRaw(this, &SNaInventoryCanvas::SlotMouseButtonUpToUMG);
		OnSlotMouseMove.AddRaw(this, &SNaInventoryCanvas::SlotMouseMoveToUMG);
		OnSlotDoubleClicked.AddRaw(this, &SNaInventoryCanvas::SlotDoubleClickedToUMG);
	}
}
