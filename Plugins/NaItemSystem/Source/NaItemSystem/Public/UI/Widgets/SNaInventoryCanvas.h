// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Components/NaItemInventoryComponent.h"
#include "UI/Widgets/SNaItemSlot.h"

class UNaGameModeItemSystemComponent;
class UNaItemInventoryComponent;
class UNaInventoryCanvas;

/* Delegates */
DECLARE_MULTICAST_DELEGATE_OneParam(FSNaInventoryCanvasSlotEvent, int);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FSNaInventoryCanvasSlotPointerEvent, int, const FGeometry&, const FPointerEvent&);


/**
 * SNaInventoryCanvas - Slate widget that displays inventory slots on a canvas panel.
 * Unlike SNaItemSlotList which uses SWrapBox for grid layout, this widget uses SCanvas
 * and positions each slot via GetSlotPosition(), allowing arbitrary slot placement.
 * Override GetSlotPosition() in a subclass to customize slot layout.
 */
class NAITEMSYSTEM_API SNaInventoryCanvas : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaInventoryCanvas)
	{
		_StylePtr = nullptr;
		_Container = nullptr;
		_SlotAmount = -1;
		_FromUMG = nullptr;
	}

	SLATE_ATTRIBUTE(FNaItemSlotStyle*, StylePtr)
	SLATE_ATTRIBUTE(UNaItemInventoryComponent*, Container)
	/** Overall slot amount including disabled slots. If <= 0, uses inventory size. */
	SLATE_ATTRIBUTE(int, SlotAmount)
	SLATE_ARGUMENT(UNaInventoryCanvas*, FromUMG)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


protected:

	bool bIsConstructed = false;

	/* Input copies */
	UNaGameModeItemSystemComponent* GMComponent = nullptr;
	UNaItemInventoryComponent* Container = nullptr;
	FNaItemSlotStyle* StylePtr = nullptr;
	int SlotAmount;

	// If true, this widget will not contain anything.
	bool bIsInvalid = false;

	/* Subwidgets */
	TArray<TSharedPtr<SNaItemSlot>> Slots;
	TSharedPtr<class SCanvas> Canvas;

public:

	/* Getters */
	UNaItemInventoryComponent* GetContainer() { return Container; }
	TSharedPtr<SCanvas> GetCanvas() { return Canvas; }
	bool IsInvalid() { return bIsInvalid; }

public:

	/* Widget actions */

	/**
	 * Get the position for a slot at the given index.
	 * Override in subclasses for custom slot placement.
	 * The default implementation returns FVector2D::ZeroVector.
	 */
	virtual FVector2D GetSlotPosition(int32 Index) const;

	/**
	 * Reconstruct the widget: destroy all slots and re-add them from the container.
	 * Also works when this widget is invalid - it will recheck and try constructing.
	 */
	void Reconstruct();

	/** Reset a single slot's displayed item from the container. */
	void ResetSlot(int Position);

	/** Reset all slots' displayed items from the container. */
	void ResetAllSlots();

	/** Reset container reference and reconstruct the widget. */
	void ResetContainer(UNaItemInventoryComponent* NewContainer);

public:

	/* Selection */

	void SelectSlot(int Position);
	void UnselectAll();
	int GetSelectedPosition();

protected:

	int SelectedPosition = -1;

public:

	/* Delegates executed when slot events are detected */
	FSNaInventoryCanvasSlotEvent OnSlotPointed;
	FSNaInventoryCanvasSlotEvent OnSlotUnpointed;
	FSNaInventoryCanvasSlotEvent OnSlotSelected;
	FSNaInventoryCanvasSlotEvent OnSlotUnselected;
	FSNaInventoryCanvasSlotEvent OnSlotClicked;
	FSNaInventoryCanvasSlotEvent OnSlotHovered;
	FSNaInventoryCanvasSlotEvent OnSlotUnhovered;

	FSNaInventoryCanvasSlotPointerEvent OnSlotMouseButtonDown;
	FSNaInventoryCanvasSlotPointerEvent OnSlotMouseButtonUp;
	FSNaInventoryCanvasSlotPointerEvent OnSlotMouseMove;
	FSNaInventoryCanvasSlotPointerEvent OnSlotDoubleClicked;

protected:

	/**
	 * Bind a single slot's box-slot events (pointed, selected, clicked, mouse, etc.)
	 * directly to this canvas's delegates, using payload binding.
	 */
	void BindSlotEvents(int32 Position);

	/* Slot event handlers - bound via payload with the slot position */
	void HandleSlotPointed(int32 Position);
	void HandleSlotUnpointed(int32 Position);
	void HandleSlotSelected(int32 Position);
	void HandleSlotUnselected(int32 Position);
	FReply HandleSlotClicked(int32 Position);
	void HandleSlotHovered(int32 Position);
	void HandleSlotUnhovered(int32 Position);

	FReply HandleSlotMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, int32 Position);
	FReply HandleSlotMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, int32 Position);
	FReply HandleSlotMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, int32 Position);
	FReply HandleSlotDoubleClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, int32 Position);

public:

	/* UMG interface */

	void SlotPointedToUMG(int Position);
	void SlotUnpointedToUMG(int Position);
	void SlotSelectedToUMG(int Position);
	void SlotUnselectedToUMG(int Position);
	void SlotClickedToUMG(int Position);
	void SlotHoveredToUMG(int Position);
	void SlotUnhoveredToUMG(int Position);

	void SlotMouseButtonDownToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void SlotMouseButtonUpToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void SlotMouseMoveToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	void SlotDoubleClickedToUMG(int Position, const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

protected:

	TWeakObjectPtr<UNaInventoryCanvas> UMGRef = nullptr;

	void BindEventsToUMG();

};
