// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "NaItemContainer.h"
#include "Widgets/BoxSlots/SNaSlotList.h"
#include "UI/Widgets/SNaItemSlot.h"
#include "../../Components/NaItemContainerComponent.h"

class UNaGameModeItemSystemComponent;
class UNaItemContainerComponent;
class UNaItemSlotList;
/**
 * 
 */
class NAITEMSYSTEM_API SNaItemSlotList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaItemSlotList)
	{
		_Container = nullptr;
		_BoxSize = FVector2D(64.0, 64.0);
		_bHideAmountWhenOne = true;
		_Font = FTextBlockStyle::GetDefault().Font;
		_bFillDisabledToCompleteRectangle = true;
		_RowLength = 8;
		_FromUMG = nullptr;
	}

	SLATE_ATTRIBUTE(UNaItemContainerComponent*, Container)
	SLATE_ATTRIBUTE(FVector2D, BoxSize)
	SLATE_ATTRIBUTE(bool, bHideAmountWhenOne)
	SLATE_ATTRIBUTE(FSlateFontInfo, Font)
	SLATE_ATTRIBUTE(bool, bFillDisabledToCompleteRectangle)	/* If true, it will add disabled slots to the end to fill a complete rectangle. */
	SLATE_ATTRIBUTE(int, RowLength)	/* How many boxes in a row */
	SLATE_ARGUMENT(UNaItemSlotList*, FromUMG) /* If generated from UNaItemSlotList, set this value */


	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/* Initialization after construction. Should be called manually after Construct().
	* When reconstructing after constructed, this function will be automatically called.
	*/
	void PostConstructionInit();
	FSimpleDelegate OnPostConstructionInit;

protected:

	// A tag indicating if this widget is already constructed.
	// Mainly for avoiding Reconstruction() call PostConstructionInit() on construction, which cause "this" pointer error.
	bool bIsConstructed = false;

	/* Input copies */

	UNaGameModeItemSystemComponent* GMComponent;
	UNaItemContainerComponent* Container;
	FVector2D BoxSize;
	bool bHideAmountWhenOne;
	FSlateFontInfo Font;
	bool bFillDisabledToCompleteRectangle;
	int RowLength;

	// Actual length including disabled slots in the end 
	int ActualLength;

	// If true, this widget will not contain anything. For cases in which container or game mode component is invalid.
	bool bIsInvalid = false;


	/* Subwidgets */
	TSharedPtr<SWrapBox> WrapBox;
	TArray<TSharedPtr<SNaItemSlot>> Slots;

public:

	/* Get functions */
	UNaItemContainerComponent* GetContainer(){ return Container; };

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
	void ResetContainer(UNaItemContainerComponent* NewContainer);

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

	// Event when a child slot is selected of pointed. Input is slot position in the list.

	// Event from a specific slot of the list, with index of the source slot
	DECLARE_DELEGATE_OneParam(FNaListSlotEvent, int);

	// List slot event for pointer events, with geometry and pointer event input
	DECLARE_DELEGATE_ThreeParams(FNaListSlotPointerEvent, int, const FGeometry&, const FPointerEvent&);
	
	/* Delegates executed when slot mouse/selection events are detected */
	FNaListSlotEvent OnSlotPointed;
	FNaListSlotEvent OnSlotUnpointed;
	FNaListSlotEvent OnSlotSelected;
	FNaListSlotEvent OnSlotUnselected;
	FNaListSlotEvent OnSlotClicked;
	FNaListSlotEvent OnSlotHovered;
	FNaListSlotEvent OnSlotUnhovered;

	FNaListSlotPointerEvent OnSlotMouseButtonDown;
	FNaListSlotPointerEvent OnSlotMouseButtonUp;
	FNaListSlotPointerEvent OnSlotMouseMove;

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

protected:

	UNaItemSlotList* UMGRef = nullptr;

	void BindEventsToUMG();

};
