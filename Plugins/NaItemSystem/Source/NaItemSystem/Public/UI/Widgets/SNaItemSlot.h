// By Sodium

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/BoxSlots/SNaBoxSlot.h"
#include "NaItemEntry.h"

class UNaGameModeItemSystemComponent;

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

	SLATE_ATTRIBUTE(UObject*, WorldContext)
	SLATE_ATTRIBUTE(TSharedPtr<FNaItemEntry>, EntryPtr)	/* Null ptr means empty */
	SLATE_ATTRIBUTE(FVector2D, Size)
	SLATE_ATTRIBUTE(bool, bHideAmountWhenOne)
	SLATE_ATTRIBUTE(bool, bIsDisabled) /* If true, this slot will be regarded as disabled, ignoring the value of ItemEntry and bIsEmpty */
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
};
