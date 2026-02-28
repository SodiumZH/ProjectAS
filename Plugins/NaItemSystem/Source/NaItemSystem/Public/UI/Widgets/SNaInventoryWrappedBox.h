// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Components/NaItemInventoryComponent.h"
#include "UI/Widgets/SNaItemSlot.h"

class UNaGameModeItemSystemComponent;
class UNaItemInventoryComponent;
class UNaItemSlotList;
/**
 * 
 */
/* Delegates */
 // Event when a child slot is selected of pointed. Input is slot position in the list.
 // Event from a specific slot of the list, with index of the source slot
DECLARE_MULTICAST_DELEGATE_OneParam(FSNaItemListSlotEvent, int);

// List slot event for pointer events, with geometry and pointer event input
DECLARE_MULTICAST_DELEGATE_ThreeParams(FSNaItemListSlotPointerEvent, int, const FGeometry&, const FPointerEvent&);


class NAITEMSYSTEM_API SNaInventoryWrappedBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaInventoryWrappedBox)
	{
		_StylePtr = nullptr;
		_Container = nullptr;
		_bFillDisabledToCompleteRectangle = true;
		_RowLength = 8;
		_FromUMG = nullptr;
	}

	SLATE_ATTRIBUTE(FNaItemSlotStyle*, StylePtr)
	SLATE_ATTRIBUTE(UNaItemInventoryComponent*, Container)
	SLATE_ATTRIBUTE(bool, bFillDisabledToCompleteRectangle)	/* If true, it will add disabled slots to the end to fill a complete rectangle. */
	SLATE_ATTRIBUTE(int, RowLength)	/* How many boxes in a row */
	SLATE_ATTRIBUTE(int, RowCount)	// Amount of rows. If this count is not enough for container, it will be ignored and the row count will be automatically calculated from container.
	SLATE_ARGUMENT(UNaItemSlotList*, FromUMG) /* If generated from UNaItemSlotList, set this value */

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


protected:

	// A tag indicating if this widget is already constructed.
	// Mainly for avoiding Reconstruction() call PostConstructionInit() on construction, which cause "this" pointer error.
	bool bIsConstructed = false;

	/* Input copies */

	UNaGameModeItemSystemComponent* GMComponent;
	UNaItemInventoryComponent* Container;
	FNaItemSlotStyle* StylePtr = nullptr;
	bool bFillDisabledToCompleteRectangle;
	int RowLength;
	int RowCount;

	// Actual length including disabled slots in the end 
	int ActualLength;

	// If true, this widget will not contain anything. For cases in which container or game mode component is invalid.
	bool bIsInvalid = false;


	/* Subwidgets */
	
	TArray<TSharedPtr<SNaItemSlot>> Slots;
	TSharedPtr<class SWrapBox> WrapBox;

public:

	/* Get functions */
	UNaItemInventoryComponent* GetContainer(){ return Container; };

	TSharedPtr<SWrapBox> GetWrapBox() {	return WrapBox; };

public:

	/* Widget state related */

	bool IsInvalid() { return bIsInvalid; };

public:

	/* Widget action related */

	// Check if the widget content (child item slots) is updated to match the container's content.
	// Return true if all match.
	// If this function returns false, it means some changes happened in the container but not updated to widget content.
	// @Param bWarning If true, it will print information to log when failed.
	bool IsUpdated(bool bDisplay = false);

	/* Get the current state of container and reconstruct the widget.
	* It will destroy all slots and re-add them.
	* It also works when this widget is invalid. It will recheck if it's still invalid, and try constructing when not.
	*
	* When only some positions change but the whole container doesn't (i.e. no resize, container reference change,...) ,
	* Please call ResetSlot() or ResetAllSlots() instead since it is much faster (no widget creating & deleting).
	*
	* Cases below are when this function is needed:
	* (a) Container reference changed; (b) Container is resized; (c) This widget is invalid and need construction.
	*/
	void Reconstruct();

	/* Reset a slot of given position from container.
	* Please ensure the container reference and size didn't change, or unexpected behaviors may happen.
	*/
	void ResetSlot(int Position);

	/* Reset all slots. 
	* If size changed, use Reconstruct() instead.
	*/
	void ResetAllSlots();

	/* Reset container reference, and reconstruct widget */
	void ResetContainer(UNaItemInventoryComponent* NewContainer);

public:

	/* Selection related */
	
	// Select a slot. DO NOT SET POSITIONS = -1!!! Use UnselectAll() instead
	void SelectSlot(int Position);

	// Unselect all slots
	void UnselectAll();


protected:

	// Current selected position. -1 means no slot is selected.
	int SelectedPosition = -1;


public:

	// Get current selected position. -1 means no slot is selected.
	int GetSelectedPosition();

	/* Child slot events */


	
	/* Delegates executed when slot mouse/selection events are detected */
	FSNaItemListSlotEvent OnSlotPointed;
	FSNaItemListSlotEvent OnSlotUnpointed;
	FSNaItemListSlotEvent OnSlotSelected;
	FSNaItemListSlotEvent OnSlotUnselected;
	FSNaItemListSlotEvent OnSlotClicked;
	FSNaItemListSlotEvent OnSlotHovered;
	FSNaItemListSlotEvent OnSlotUnhovered;

	FSNaItemListSlotPointerEvent OnSlotMouseButtonDown;
	FSNaItemListSlotPointerEvent OnSlotMouseButtonUp;
	FSNaItemListSlotPointerEvent OnSlotMouseMove;
	FSNaItemListSlotPointerEvent OnSlotDoubleClicked;

public:

	/* UNaItemSlotList interface */

	// Called when slot selection/mouse events are detected and execute UMG's slot selection/mouse events
	// This will occupy "OnSlot" events. Custom behaviors should be set in the UMG if the UMG exists.
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

	TWeakObjectPtr<UNaItemSlotList> UMGRef = nullptr;

	void BindEventsToUMG();

	/* End UNaItemSlotList interface */

};
