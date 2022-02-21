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
		_bIsEmpty = false;
	}

	SLATE_ATTRIBUTE(UObject*, WorldContext)
	SLATE_ATTRIBUTE(TSharedPtr<FNaItemEntry>, EntryPtr)
	SLATE_ATTRIBUTE(FVector2D, Size)
	SLATE_ATTRIBUTE(bool, bHideAmountWhenOne)
	SLATE_ATTRIBUTE(bool, bIsEmpty)		/* If true, this slot will be regarded as empty, ignoring the value of ItemEntry */
	SLATE_ATTRIBUTE(bool, bIsDisabled) /* If true, this slot will be regarded as disabled, ignoring the value of ItemEntry and bIsEmpty */

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


protected:

	UNaGameModeItemSystemComponent * GMComponent;

	FNaItemEntry ItemEntry;

	bool bIsEmpty;

	bool bIsDisabled;

	bool bHideAmountWhenOne;

	TSharedPtr<SNaBoxSlot> BoxSlot;

public:



};
