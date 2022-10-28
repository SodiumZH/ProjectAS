// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "NaItemEntry.h"

class UNaGameModeItemSystemComponent;
class SNaItemSlotList;


/**
 * 
 */
class NAITEMSYSTEM_API SNaItemSlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SNaItemSlot)
	{
		_WorldContext = nullptr;
		_EntryPtr = nullptr;
		_Size = FVector2D(64.0, 64.0);
		_bHideAmountWhenOne = true;
		_bIsDisabled = false;
		_Font = FTextBlockStyle::GetDefault().Font;
	}

	SLATE_ATTRIBUTE(UObject*, WorldContext)	// object as world context (indicating world)
	SLATE_ATTRIBUTE(TSharedPtr<FNaItemEntry>, EntryPtr)	/* Ptr to corresponding item entry. Null ptr means empty */
	SLATE_ATTRIBUTE(FVector2D, Size) // Count of slots
	SLATE_ATTRIBUTE(bool, bHideAmountWhenOne)	// If true, the item amount sub/superscript will be hidden when amount is 1.
	SLATE_ATTRIBUTE(bool, bIsDisabled) /* If true, this slot will be regarded as disabled, ignoring the value of ItemEntry and bIsEmpty() */
	SLATE_ATTRIBUTE(FSlateFontInfo, Font)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/* Make slot params from item entry using this item slot's context */
	void ParamsFromEntry(FNaBoxSlotParams& OutParams);

protected:

	// Only for setting slot. Temporary param.
	FNaBoxSlotParams TempParams;

protected:

	/* Subwidget */

	TSharedPtr<SNaBoxSlot> BoxSlot;

	/* State and general properties */

	bool bIsDisabled;

	bool bHideAmountWhenOne;

	FSlateFontInfo Font;

	FVector2D Size;

	/* Properties for getting item information */

	UNaGameModeItemSystemComponent * GMComponent;

	TSharedPtr<FNaItemEntry> ItemEntryPtr;

public:

	// Check if it's an empty slot
	bool IsEmpty() { return !ItemEntryPtr.IsValid(); };

public:

	// Get item entry ptr
	TSharedPtr<FNaItemEntry> GetEntryPtr() { return ItemEntryPtr; };

	// Get child as box slot
	TSharedPtr<SNaBoxSlot> GetBoxSlot() { return BoxSlot; };

	// Get a new item entry ptr and reset the slot from it
	void ResetItemEntry(TSharedPtr<FNaItemEntry> NewEntryPtr);

	// Set if the slot is disabled
	void SetDisabled(bool NewDisabledState);

protected:

	/**************************************************/
	/* Functions for interaction with SNaItemSlotList */

	/* If this ptr is null, this slot is not in an SNaItemSlotList
	*/
	TWeakPtr<SNaItemSlotList> ItemSlotList = nullptr;

	int PositionInSlotList = -1;

	/* Bind this slot's pointing and selecting state to item slot list */
	void BindItemSlotListEvents();

public:

	/* Functions for transfer events to ItemSlotList.
	* Warning: no validity check inside. Check validity before calling.
	*/
	void SlotPointedToList();
	void SlotUnpointedToList();
	void SlotSelectedToList();
	void SlotUnselectedToList();
	FReply SlotClickedToList();
	void SlotHoveredToList();
	void SlotUnhoveredToList();

	FReply SlotMouseButtonDownToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	FReply SlotMouseButtonUpToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	FReply SlotMouseMoveToList(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	// Empty function when no item slot list.
	void ExecNoList() { return; };
	FReply ExecNoListClicked() { return FReply::Handled(); };
	FReply ExecNoListMouse(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) { return FReply::Handled(); };


	// Call this function ONLY when constructing slots in item slot list.
	void SetItemSlotList(TSharedPtr<SNaItemSlotList> List, int Position);

	/* SNaItemSlotList end */
	/***********************/

};
