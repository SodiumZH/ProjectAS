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
	}

	SLATE_ATTRIBUTE(UNaItemContainerComponent*, Container)
	SLATE_ATTRIBUTE(FVector2D, BoxSize)
	SLATE_ATTRIBUTE(bool, bHideAmountWhenOne)
	SLATE_ATTRIBUTE(FSlateFontInfo, Font)
	SLATE_ATTRIBUTE(bool, bFillDisabledToCompleteRectangle)	/* If true, it will add disabled slots to the end to fill a complete rectangle. */
	SLATE_ATTRIBUTE(int, RowLength)	/* How many boxes in a row */

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:


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
	
	// Select a slot
	void SelectSlot(int Position);

	// Unselect all slots
	void UnselectAll();


protected:

	// Current selected position. -1 means no slot is selected.
	int SelectedPosition = -1;


public:

	/* Item operation related */

};
